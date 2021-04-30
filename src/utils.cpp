#include "utils.h"

#include <Arduino.h>

/** Generate an HTML response for a Markdown string */
String utils::markdown(String mk) {
  return (
    "<!doctype html>"
    "<html>"
    "<head>"
    "  <meta charset=\"utf-8\"/>"
    "</head>"
    "<body>"
    "  <div id=\"content\"><code><noscript style=\"white-space: pre;\">" + mk + "</noscript></code></div>"
    "  <script src=\"https://cdn.jsdelivr.net/npm/marked/marked.min.js\"></script>"
    "  <script>"
    "    document.getElementById('content').innerHTML = marked(document.getElementById('content').children[0].children[0].innerHTML);"
    "  </script>"
    "</body>"
    "</html>"
  );
}

void utils::Beep::start() {
  if (beepTone > 0) {
    tone(buzzerPin, beepTone);
  }

  startTime = millis();
}

void utils::Beep::stop() {
  if (beepTone > 0) {
    noTone(buzzerPin);
  }
}

bool utils::Beep::started() {
  return startTime != 0;
}

bool utils::Beep::shouldStop() {
  return millis() > startTime + beepDuration;
}
