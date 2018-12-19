#include <EEPROM.h>
#include "menuEnums.h"
#include "support.h"
#include "eepromstore.h"
#include "UltraSonicSensor.h"
#include "Buzzer.h"
#include "Indicator.h"
#include "serialsupport.h"
#include "ParkingControl.h"


//SINCE ARDUINO UNO ALWAYS RESETS ON "Serial Request", Always start in plot mode
/// Variable only for debugg purpouses
bool plot_mode = true;

//Create static instances of classes
UltraSonicSensor sensor(Support::pinning.TRIG_PIN, Support::pinning.ECHO_PIN);
Indicator indicator(Support::pinning.INDICATOR_PIN);
Buzzer buzzer(Support::pinning.BUZZER_PIN);
ParkingControl control(&sensor);


//Alocate storage 
struct eepromStore storage;
struct eepromStore runtimeStorage;

///Tweek this!
// MIN "10" for sensor stability (determined by testing)
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
    control.setAlgorithm(runtimeStorage.selectedAlgorithm);
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

    int selection = -1;
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

        case MENU_SERVICE_CHANGE_ALGO:
            //Print posible algos:

            Serial.println(F("Input Algo: "));
            selection = serialGetInt(true);

            if (control.setAlgorithm(selection)) //valid 
            {
                runtimeStorage.selectedAlgorithm = selection;
                Serial.println(F("Valid Algorithm! Rember to store to EEPROM!"));
            }
            else 
            {
                Serial.println(F("Invalid Selection, Try Again"));
            }
            
            break;

        case MENU_SERVICE_SET_ALARM_FREQUENCY:
            Serial.println(F("Input Frequency: "));
            selection = serialGetInt(true);
            if (selection < 0)
            {
                Serial.println(F("Invalid Frequency, Try Again"));
            }
            else
            {
                runtimeStorage.alarmFrequency = selection;
                Serial.println(F("Valid Frequency! Rember to store to EEPROM!"));
            }

            break;

        case MENU_SERVICE_SET_ALARM_DURATION:
            Serial.println(F("Input Duration in ms: "));
            selection = serialGetInt(true);
            if (selection < 0)
            {
                Serial.println(F("Invalid Duration, Try Again"));
            }
            else
            {
                runtimeStorage.alarmDuration = selection;
                Serial.println(F("Valid Duration! Rember to store to EEPROM!"));
            }

            break;
            
        case MENU_SERVICE_SET_NORMAL_DISTANCE:
            Serial.println(F("Input Normal Distance in cm's: "));
            selection = serialGetInt(true);
            if (selection < 0)
            {
                Serial.println(F("Invalid Distance, Try Again"));
            }
            else
            {
                runtimeStorage.normalDistance = selection;
                Serial.println(F("Valid Distance! Rember to store to EEPROM!"));
            }

            break;

        default:
            Serial.println(F("Unrecognized command"));
            printServiceMenu();
            break;

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
        //1. unsilience all modules that print
        indicator.setPrint(true);
        buzzer.setPrint(true);
        //2. disable mesurment serialPrintConfiguration
        sensor.setPrint(false);
    }
    
}


void ISRHandler() 
{
    return;
}
