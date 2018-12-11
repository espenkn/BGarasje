#ifndef BB_SERIAL_SUPPORT
#define BB_SERIAL_SUPPORT

#include "Arduino.h"

int incomingByte = 0;


void printDebugCmds() {
    
    Serial.println(F("CMDS:"));
    Serial.println(F("alarm on|off"));
    Serial.println(F("indicator on|off"));
    Serial.println(F("debug off / exit")); //Or exit

    Serial.println();
    Serial.flush();

}

void printRunCmds() {
    
    Serial.println(F("CMDS:"));
    Serial.println(F("debug on|off"));
    Serial.println(F("print on|off"));
    Serial.println(F("threshold set|get|store|eeprom"));
    Serial.println();
    Serial.flush();
    
}

#endif
