#include "main.h"

#include <ESP8266WiFi.h>
#include <uri/UriRegex.h>

void setup(void) {
    /* Start serial communication at 115200 bauds */
  Serial.begin(115200);
  Serial.println("\n");

  Serial.println("bleep  Copyright (C) 2021  Nurrl@github.com and contributors");
  Serial.println("This program comes with ABSOLUTELY NO WARRANTY; for details");
  Serial.println("see GPLV3+ licence file. This is free software, and you are ");
  Serial.println("welcome to redistribute it under certain terms;\n");

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
    delay(delay_step);
    if (WiFi.status() == WL_NO_SSID_AVAIL){
        Serial.println("<#> No Wi-Fi Networks are available, shutting down...");
    } else if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("<#> Connection to the Wi-Fi network failed.");
    } else if (WiFi.status() == WL_IDLE_STATUS) {
        /* Make the delay grow each time, with a maximum of 10 seconds between checks.
        Check forever if no connection is found.*/
        delay_step = (delay_step + 1 < 10000) ? delay_step + 1 : delay_step;
    }
  }

  Serial.print(" Connected as ");
  Serial.println(WiFi.localIP());

  /* Set up HTTP server endpoints */
  HTTPsetup();
}

void HTTPsetup() {
  /** [GET /] */
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
        "### **GET** `/playing/{status}`\n"
        "*Start or stop the dequeuing of notes depending on the provided `status`.*\n"
        "\n"
        "##### Parameters:\n"
        "- `status`: `bool` — wether the song should be played or not.\n"
        "\n"
        "##### Examples:\n"
        "- [on](/playing/1) — `/playing/1`\n"
        "- [off](/playing/0) — `/playing/0`\n"
        "\n"
        "### **GET** `/beep/{tone}:{duration}`\n"
        "*Emit a beep or enqueue a new one (up to " + String(BEEP_QUEUE) + ") if one is currently playing.*  \n"
        "*You might want to do a pause in your notes, simply provide a `tone` of `0Hz`.*\n"
        "\n"
        "##### Parameters:\n"
        "- `tone`: `int{0|30,12000}` — the desired tone, exprimed in **Hz**.\n"
        "- `duration`: `int{50,4000}` — the desired duration, exprimed in **ms**.\n"
        "\n"
        "##### Piano keyboard:\n"
        "&nbsp; |[█](/beep/277:500)|[█](/beep/311:500)|_|[█](/beep/370:500)|[█](/beep/415:500)|[█](/beep/466:500)|  \n"
        "|[C](/beep/262:500)|[D](/beep/294:500)|[E](/beep/330:500)|[F](/beep/349:500)|[G](/beep/392:500)|[A](/beep/440:500)|[B](/beep/494:500)|\n"
      )
    );
  });

  /** [GET /playing/{status}] */
  server.on(UriRegex("^\\/playing\\/([0-1])$"), HTTP_GET, []() {
    server.sendHeader("Server", SERVER, true); /* Set `Server` header */

    /* Extract values from the path parameters */
    bool status = server.pathArg(0).toInt();

    /* Set playing status */
    playing = status;

    Serial.println("<i> Set the playing status to `" + String(playing) + "`.");

    /* Return `204 No Content` to the client */
    server.send(204);
  });

  /** [GET /beep/{tone}:{duration}] */
  server.on(UriRegex("^\\/beep\\/([0-9]+):([0-9]+)$"), HTTP_GET, []() {
    server.sendHeader("Server", SERVER, true); /* Set `Server` header */

    /* Extract values from the path parameters */
    int hz = server.pathArg(0).toInt();
    int duration = server.pathArg(1).toInt();

    /* Check for ranges of the `hz` and `duration` variables */
    if (hz != 0 && (hz < 30 || hz > 12000)) {
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

    Serial.println("<i> Enqueued a new beep (" + String(hz) + "Hz, " + String(duration) + "ms), "
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
  if (playing && beep && !beep->started()) {
    beep->start();

    Serial.println("<!> Beeping at " + String(beep->beepTone) + "Hz for " + String(beep->beepDuration) + "ms.");
  }

  /* Loop over the server to handle the clients one by one */
  server.handleClient();

  /* Check if a timer expired to stop the beep */
  if (beep && beep->started() && beep->shouldStop()) {
    beeps.remove(0);
    beep->stop();

    delete beep;
  }
}
