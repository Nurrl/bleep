#pragma once

#include <Arduino.h>
#include <WString.h>

namespace utils {
  String markdown(String mk);

  class Beep {
    public:
      Beep(short pin, unsigned short hz, unsigned short duration): buzzerPin(pin), beepTone(hz), beepDuration(duration), startTime(0) {}

      void start();
      void stop();

      bool started();
      bool shouldStop();
      
      unsigned short beepTone;
      unsigned short beepDuration;

    private:
      short buzzerPin;

      unsigned long startTime;
  };
}
