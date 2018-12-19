#ifndef BB_SUPPORT
#define BB_SUPPORT

#include "Arduino.h"
#include "eepromstore.h"

class Support
{
    public: 
        static const int defaultThreshold = 200;
        static const bool defaultAlarmSetting = false;
        static const int defaultAlarmFrequency = 800;
        static const int defaultAlarmDuration = 2000; //2 Sec
        static const int defaultNormalDistance = 200;
        static const int defaultHeadroom = 100;

        static const struct PinConfig {
            static const char TRIG_PIN = 4;
            static const char ECHO_PIN = 6;
            static const char BUZZER_PIN = 9;
            static const char INDICATOR_PIN = 11;
            //static const char INTERUPT_PIN = 2;

        } pinning;

        void static sanitizeStorageWithDefaults(eepromStore& store, bool print);


};

void Support::sanitizeStorageWithDefaults(eepromStore& store, bool print)
{

    if (store.threshold < 1) 
    {
        if (print) {
            Serial.println(F("Bad sensor threshold setting. Loading default. Please calibrate and store to eeprom.")); 
        }
        
        store.threshold = Support::defaultThreshold;
    }

    if (store.alarmEnabeld == false) 
    { 
        if (print) {
            Serial.println(F("Alarm is disabled"));
        }
    }    

    if (store.normalDistance < 1) 
    {
        if (print) {
            Serial.println(F("Bad sensor \"normal distance\" setting. Loading default. Please calibrate and store to eeprom.")); 
        }
        store.normalDistance = Support::defaultNormalDistance;
    }

    if (store.alarmDuration < 1) 
    {
        if (print) {
            Serial.println(F("Alarm duration not set. Loading default")); 
        }
        store.alarmDuration = Support::defaultAlarmDuration;
    }

    if (store.alarmFrequency < 1) 
    {
        if (print) {
            Serial.println(F("Alarm frequency not set. Loading default")); 
        }
        store.alarmFrequency = Support::defaultAlarmFrequency;
    }


    Serial.flush();
}

#endif
