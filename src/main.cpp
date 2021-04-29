#include "main.h"

#include <ESP8266WiFi.h>
#include <uri/UriRegex.h>

void setup(void) {
    /* Start serial communication at 115200 bauds */
  Serial.begin(115200);
  Serial.println("");

  Serial.println("<#> Starting the Bleep v" + String(VERSION) + " Internet of Shit server !");

  /* Set leds and buzzer to output mode and set them to low */
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);

  /* Connect to the WiFi hotspot specified in the configuration */
  WiFi.begin(ssid, passwd);

  Serial.print("<#> Waiting for Wi-Fi connection..");

  /* Loop over the Wi-Fi connection check */
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }

  Serial.print(" Connected as ");
  Serial.println(WiFi.localIP());

  /* Set up HTTP server endpoints */
  HTTPsetup();
}

void HTTPsetup() {
  /** [GET /]
   * Root page, displaying introduction and route specs for the API.
   */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Server", SERVER, true); /* Set `Server` header */

    server.send(
      200,
      "text/html",
      utils::markdown(
        "# I'm a Bleep v" VERSION " server !\n"
        "I'm an **IoS**(for Internet of Shit) device that does *bleeps* and *bloops* on a buzzer, with the below endpoints.\n"
        "\n"
        "### **GET** `/`\n"
        "*Get this **wonderful** explanatory page.*\n"
        "\n"
        "### **GET** `/bleep/{tone}:{duration}`\n"
        "*Emit a beep or enqueue a new one (up to " + String(BEEP_QUEUE) + ") if one is currently playing.*\n"
        "\n"
        "##### Parameters:\n"
        "- `tone`: `int{30,12000}` — the desired tone, exprimed in **Hz**.\n"
        "- `duration`: `int{50,4000}` — the desired duration, exprimed in **ms**.\n"
        "\n"
        "##### Piano keyboard:\n"
        "&nbsp; |[█](/bleep/277:500)|[█](/bleep/311:500)|||[█](/bleep/370:500)|[█](/bleep/415:500)|[█](/bleep/466:500)|  \n"
        "|[C](/bleep/262:500)|[D](/bleep/294:500)|[E](/bleep/330:500)|[F](/bleep/349:500)|[G](/bleep/392:500)|[A](/bleep/440:500)|[B](/bleep/494:500)|\n"
      )
    );
  });

  /** [GET /bleep/{tone}:{duration}]
   * Produce a beep with the provided buzzer with the frequency of `tone` Hz during `duration` ms.
   */
  server.on(UriRegex("^\\/bleep\\/([0-9]+):([0-9]+)$"), HTTP_GET, []() {
    server.sendHeader("Server", SERVER, true); /* Set `Server` header */

    /* Extract values from the path parameters */
    int hz = server.pathArg(0).toInt();
    int duration = server.pathArg(1).toInt();

    /* Check for ranges of the `hz` and `duration` variables */
    if (hz < 30 || hz > 12000) {
      /* Return `400 Bad Request` if the tone is out of range */
      server.send(400, "text/plain", "The provided tone is out of range !");
      return;
    }
    if (duration < 50 || duration > 4000) {
      /* Return `400 Bad Request` if the duration is out of range */
      server.send(400, "text/plain", "The provided duration is out of range !");
      return;
    }

    /* Check if a new beep can be pushed to the Array */
    if (beeps.size() >= BEEP_QUEUE) {
      /* Return `425 Too Early` if the queue is full */
      server.send(425, "text/plain", "Unfortunately the queue of beeps is full, try again later !");
      return;
    }

    /* Store the beep to the list */
    beeps.add(new Beep(buzzer, hz, duration));

    Serial.println("<i> Successfully enqueued a new beep (" + String(hz) + "Hz, " + String(duration) + "ms), "
      "queue is at " + String(beeps.size()) + "/" + String(BEEP_QUEUE) + ".");

    /* Return `204 No Content` to the client */
    server.send(204);
  });

  server.onNotFound([]() {
    server.sendHeader("Server", SERVER, true); /* Set `Server` header */
    server.send(404, "text/plain", "Are you lost !?");
  });

  /* Finally start the server */
  server.begin();
  Serial.print("<#> Started HTTP server on `http://");
  Serial.print(WiFi.localIP());
  Serial.println(":80`");
}

void loop(void) {
  /* Get the current beep and starts it if required */
  Beep* beep = beeps.get(0);
  if (beep && !beep->started()) {
    beep->start();
    Serial.println("<!> Beeping at " + String(beep->beepTone) + "Hz for " + String(beep->beepDuration) + "ms.");
  }

  /* Loop over the server to handle the clients one by one */
  server.handleClient();

  /* Check if a timer expired to stop the beep */
  if (beep && beep->shouldStop()) {
    beeps.remove(0);
    beep->stop();

    delete beep;
  }
}
