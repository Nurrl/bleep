#pragma once

#include <WString.h>

namespace utils {
  String markdown(String mk);

  class Beep {
    public:
      Beep(short pin, unsigned short hz, unsigned short duration): beepTone(hz), beepDuration(duration), buzzerPin(pin), startTime(0) {}

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
