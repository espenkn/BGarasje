#ifndef BB_SERIAL_SUPPORT
#define BB_SERIAL_SUPPORT

#include "Arduino.h"
#include "menuEnums.h"

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


void printMenu()
{
    Serial.print(F("\n\n"));
    Serial.println(F("Menu:"));

    Serial.print(MENU_PRINT_ON, DEC);
    Serial.println(F(": Print Mesurments - Enable"));

    Serial.print(MENU_PRINT_OFF, DEC);
    Serial.println(F(": Print Mesurments - Disable"));
    
    Serial.print(MENU_PLOT_ON, DEC);
    Serial.println(F(": Plotting Mode - Enable"));

    Serial.print(MENU_PLOT_OFF, DEC);
    Serial.println(F(": Exit Plotting - Disable"));

    Serial.print(MENU_THRESHOLD_SET, DEC);
    Serial.println(F(": Threshold Set Runtime Value"));
    
    Serial.print(MENU_THRESHOLD_SET, DEC);
    Serial.println(F(": Threshold Get Runtime Value"));

    Serial.print(MENU_THRESHOLD_STORE, DEC);
    Serial.println(F(": Threshold Store To EEPROM"));

    Serial.print(MENU_THRESHOLD_EEPROM, DEC);
    Serial.println(F(": Threshold Get Stored Value From EEPROM"));

    Serial.print(MENU_PRINT_CONFIG, DEC);
    Serial.println(F(": Print Configuration"));

    Serial.print(MENU_ENTER_SERVICE, DEC);
    Serial.println(F(": Enter Service Menu"));

    
    Serial.flush();
}

void printServiceMenu()
{
    Serial.print(F("\n\n"));
    Serial.println(F("Service Menu:"));

    Serial.print(MENU_SERVICE_EXIT, DEC);
    Serial.println(F(": Exit Service Mode"));

    Serial.print(MENU_SERVICE_ALARM_ON, DEC);
    Serial.println(F(": Alarm On"));

    Serial.print(MENU_SERVICE_ALARM_OFF, DEC);
    Serial.println(F(": Alarm Off (Force)"));

    Serial.print(MENU_SERVICE_INDICATOR_ON, DEC);
    Serial.println(F(": Indicator On"));

    Serial.print(MENU_SERVICE_INDICATOR_OFF, DEC);
    Serial.println(F(": Indicator Off"));

    Serial.print(MENU_SERVICE_CLEAR_EEPROM, DEC);
    Serial.println(F(": Clear EEPROM"));

    Serial.flush();
}

void serialPrintConfiguration(struct eepromStore runtimeStorage, struct eepromStore storage)
{
    Serial.print(F("\n\n"));
    
    Serial.println(F("**** Active Configuration:"));
    Serial.print(F("Threshold value:"));
  
    Serial.print(runtimeStorage.threshold);
    Serial.println();
  
    Serial.print(F("Alarm Enabled:"));
    Serial.println(runtimeStorage.alarmEnabeld);
    
    Serial.println();
    
    Serial.println(F("**** Saved Configuration:"));
    Serial.print(F("Threshold value:"));
  
    Serial.print(storage.threshold);
    Serial.println();
  
    Serial.print(F("Alarm Enabled:"));
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
