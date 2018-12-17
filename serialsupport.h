#ifndef BB_SERIAL_SUPPORT
#define BB_SERIAL_SUPPORT

#include "Arduino.h"

void printDebugCmds() 
{
    
    Serial.println(F("CMDS:"));
    Serial.println(F("alarm on|off"));
    Serial.println(F("indicator on|off"));
    Serial.println(F("debug off / exit")); //Or exit

    Serial.println();
    Serial.flush();

}

void printRunCmds() 
{
    
    Serial.println(F("CMDS:"));
    Serial.println(F("debug on|off"));
    Serial.println(F("print on|off"));
    Serial.println(F("plot on|off"));
    Serial.println(F("threshold set|get|store|eeprom"));
    Serial.println(F("print config"));
    Serial.println();
    Serial.flush();
    
}


void serialPrintConfiguration(struct eepromStore runtimeStorage, struct eepromStore storage)
{
    Serial.print(F("\n\n"));
    
    Serial.println(F("**** Active Configuration:"));
    Serial.print(F("Threshold value:"));
  
    Serial.print(runtimeStorage.threshold);
    Serial.println();
  
    Serial.print(F("Alarm Enabeld:"));
    Serial.println(runtimeStorage.alarmEnabeld);
    
    Serial.println();
    
    Serial.println(F("**** Saved Configuration:"));
    Serial.print(F("Threshold value:"));
  
    Serial.print(storage.threshold);
    Serial.println();
  
    Serial.print(F("Alarm Enabeld:"));
    Serial.println(storage.alarmEnabeld);
    
    Serial.flush();
}

int serialGetInt()
{
    static int incomingByte = 0;
    
    // send data only when you receive data:
    if (Serial.available() > 0) {
        // read the incoming byte:
        incomingByte = Serial.parseInt();
        
        // say what you got:
        Serial.print(F("<<: "));
        Serial.println(incomingByte, DEC);
        Serial.println();
        
        Serial.flush();
        
        return incomingByte;
        
    } 

    return -1;
}


#endif
