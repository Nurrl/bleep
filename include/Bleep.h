#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <uri/UriRegex.h>

#include <LinkedList.h>

#include "config.h"
#include "utils.h"

using utils::Beep;

#define VERSION "1.0"
#define SERVER "Bleep/" VERSION

#ifndef IWSSID
# error "The Wi-Fi SSID is not defined in the 'config.h' file !."
#endif

#ifndef IWPASSWD
# error "The Wi-Fi Password is not defined in the 'config.h' file !."
#endif

LinkedList<Beep*> beeps;

const char* ssid = IWSSID;
const char* passwd = IWPASSWD;

ESP8266WebServer server(80);

const int led0 = LED0_PIN;
const int led1 = LED1_PIN;
const int buzzer = BUZZER_PIN;
