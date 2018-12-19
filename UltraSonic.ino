#include <EEPROM.h>
#include "menuEnums.h"
#include "support.h"
#include "eepromstore.h"
#include "UltraSonicSensor.h"
#include "Buzzer.h"
#include "Indicator.h"
#include "serialsupport.h"
#include "ParkingControl.h"


//Always start in plot_mode 
// Should i change tihis to eeprom? Will result in a lot of writes when toggling 
bool plot_mode = true;

UltraSonicSensor sensor(Support::pinning.TRIG_PIN, Support::pinning.ECHO_PIN);
Indicator indicator(Support::pinning.INDICATOR_PIN);
Buzzer buzzer(Support::pinning.BUZZER_PIN);
ParkingControl control(&sensor);



struct eepromStore storage;
struct eepromStore runtimeStorage;

///Tweek or remove?
#define LOOPDELAY 10

void setup() {

    // Restore EEPROM storage
    restoreStorage(storage);
    
    Serial.begin(9600); // Starts the serial communication

    // Copy storage 
    runtimeStorage = storage;

    // Handle bad configuration....
    Support::sanitizeStorageWithDefaults(runtimeStorage, !plot_mode);

    if (plot_mode) 
    {
        control.setPrint(true);
        
    } 
    else 
    {
        // Print current configuration on serial
        serialPrintConfiguration(runtimeStorage, storage);
        printMenu();
    }
    
    buzzer.setFreqency(runtimeStorage.alarmFrequency);
    buzzer.setDuration(runtimeStorage.alarmDuration);
    buzzer.enable(runtimeStorage.alarmEnabeld);

    control.setThreshold(runtimeStorage.threshold);

}

void loop() {
    // put your main code here, to run repeatedly:
    control.iterateStatemachine();

    runMenu(serialGetInt(false));

    delay(LOOPDELAY);
    
}

int thresholdHelper()
{
    Serial.print(F("Enter threshold value in cm: "));
    //String newThresholdString = Serial.readStringUntil("\n");
    //int newThreshold =  atoi( newThresholdString.c_str() );
 
    int newThreshold = serialGetInt(true);
    
    Serial.println();
    Serial.print("Got value: ");
    Serial.println(newThreshold, DEC);
    Serial.println("Rember to store configuration to EEPROM!");

    return newThreshold;
}

void runMenu(int option)
{
    if (option < 0) 
    {
        return;
    }

    switch (option) 
    {

        case MENU_STOP_PLOT_MODE:
            plot_mode = false;
            control.setPrint(false);
            break;
        case MENU_PRINT_ON:
            Serial.print("Start print");
            sensor.setPrint(true);
            break;
        
        case MENU_PRINT_OFF:
            sensor.setPrint(false);
            break;
        
        case MENU_PLOT_ON:
            plotPrint(true);
            break;
        
        case MENU_PLOT_OFF:
            plotPrint(false);
            break;
        
        case MENU_THRESHOLD_SET:
            runtimeStorage.threshold = thresholdHelper();
            control.setThreshold(runtimeStorage.threshold); //update control
            break;
        
        case MENU_THRESHOLD_GET:
            Serial.print(F("Threshold value in cm: "));
            Serial.println(runtimeStorage.threshold, DEC);
            break;
        
        
        case MENU_THRESHOLD_EEPROM:
            Serial.print(F("Threshold stored in EEPROM is: "));
            Serial.println(storage.threshold, DEC);
            break;

        case MENU_SAVE_CONFIG:
            saveStorage(runtimeStorage); //Store to EEPROM 
            restoreStorage(storage); //Update copy
            Serial.println(F("Stored to EEPROM"));
            break;

        case MENU_PRINT_CONFIG:
            serialPrintConfiguration(runtimeStorage, storage);
            break;

        case MENU_ENTER_SERVICE:            
            Serial.println(F("Entering Service Mode!"));
            printServiceMenu();
            //Hand over execution to debug mode
            while(serviceMenu()){};

            break;

        default:
            Serial.println(F("Unrecognized command!"));
            printMenu();

    }

}


int serviceMenu()
{

      //Wait for input 
    for (;;) 
    {
        if (Serial.available() > 0) 
        {
            break;
        }
    }

    int option = serialGetInt(false);

    switch (option) 
    {

        case MENU_SERVICE_EXIT:
            Serial.println(F("Exiting debug mode!"));
            return 0;
            break;
        
        case MENU_SERVICE_ALARM_ON:
            buzzer.startAlarm();
            break;
        
        case MENU_SERVICE_ALARM_OFF:
            buzzer.stopAlarm();
            break;

        case MENU_SERVICE_INDICATOR_ON:
            indicator.turnOn();
            break;
        
        case MENU_SERVICE_INDICATOR_OFF:
            indicator.turnOff();
            break;

        case MENU_SERVICE_CLEAR_EEPROM:
            clearEEPROM();
            restoreStorage(storage);
            break;

        default:
            Serial.println(F("Unrecognized command"));
            printServiceMenu();


    }

    return 1;

}

void plotPrint(bool on)
{
    if (on) 
    {
        //1. silience all modules that print
        indicator.setPrint(false);
        buzzer.setPrint(false);
        //2. Enable mesurment serialPrintConfiguration
        sensor.setPrint(true);
    } 
    else
    {
        //1. silience all modules that print
        indicator.setPrint(true);
        buzzer.setPrint(true);
        //2. Enable mesurment serialPrintConfiguration
        sensor.setPrint(false);
    }
    
}


void ISRHandler() 
{
    return;
}
