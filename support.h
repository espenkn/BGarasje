#ifndef BB_SUPPORT
#define BB_SUPPORT

#include "Arduino.h"

class Support
{
    public: 
        static const int defaultThreshold = 200;
        static const bool defaultAlarmSetting = false;

        static const struct PinConfig {
            static const char TRIG_PIN = 4;
            static const char ECHO_PIN = 6;
            static const char BUZZER_PIN = 9;
            static const char INDICATOR_PIN = 11;
            //static const char INTERUPT_PIN = 2;

        } pinning;

};

#endif
