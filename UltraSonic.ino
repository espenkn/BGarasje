#include <EEPROM.h>
#include "support.h"
#include "eepromstore.h"
#include "UltraSonicSensor.h"
#include "Buzzer.h"
#include "Indicator.h"
#include "serialsupport.h"
#include "ParkingControl.h"


//debug flag
bool printMesurments = false;

UltraSonicSensor sensor(Support::pinning.TRIG_PIN, Support::pinning.ECHO_PIN);
Indicator indicator(Support::pinning.INDICATOR_PIN);
Buzzer buzzer(Support::pinning.BUZZER_PIN);
ParkingControl control(&sensor);



struct eepromStore storage;
struct eepromStore runtimeStorage;

///Tweek or remove?
#define LOOPDELAY 50

void setup() {

    // Restore EEPROM storage
    restoreStorage(storage);

    
    Serial.begin(31250); // Starts the serial communication

    // Copy storage 
    runtimeStorage = storage;

    // Handle bad configuration....
    if (runtimeStorage.threshold < 1) 
    { 
        Serial.println(F("Bad sensor calibartion. Loading default. Please run calibrate and store to eeprom.")); 
        runtimeStorage.threshold = Support::defaultThreshold;
    }

    if (runtimeStorage.alarmEnabeld == 0) 
    { 
        Serial.println(F("Alarm is disabled"));
    }    
       

    // Print current configuration on serial
    serialPrintConfiguration(runtimeStorage, storage);

    Serial.flush();

}

void loop() {
    // put your main code here, to run repeatedly:
    control.iterateStatemachine();

    handleConsole();

    delay(LOOPDELAY);
    
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

void handleConsole() 
{
    
    // reply only when you receive data:
	if (Serial.available() > 0) 
    {
		// read the incoming string:
        String data = Serial.readStringUntil("\n");

        if (data == F("debug on")) 
        {
            Serial.println(F("Entering debug mode!"));
            printDebugCmds();

            //Hand over execution to debug mode
            while(debugMode()){};
            
        } 
        else if (data == F("print on")) 
        {
            sensor.setPrint(true);

        } 
        else if (data == F("print off")) 
        {
            sensor.setPrint(false);

        } 
        else if (data == F("plot on")) 
        {
            plotPrint(true);
        }
        else if (data == F("plot off")) 
        {
            plotPrint(false);
        }
        else if (data == F("threshold set")) 
        {
            
            Serial.print(F("Enter threshold value in cm: "));
            //String newThresholdString = Serial.readStringUntil("\n");
            //int newThreshold =  atoi( newThresholdString.c_str() );
            while (!Serial.available()) 
            {
                delay(20); //wait for input
            }

            int newThreshold = Serial.parseInt();
            
            runtimeStorage.threshold = newThreshold;

            Serial.println();
            Serial.print("Got value: ");
            Serial.println(newThreshold, DEC);
            Serial.println("To SAVE to eeprom run \"threshold store\"");
            

        } 
        else if (data == F("threshold get")) 
        {
            Serial.print(F("Threshold value in cm: "));
            Serial.println(runtimeStorage.threshold, DEC);
            
        } 
        else if (data == F("threshold store"))
        {
            saveStorage(storage);
            Serial.println(F("Stored to EEPROM"));

        } 
        else if (data == F("threshold eeprom")) 
        {
            Serial.print(F("Threshold stored in EEPROM is: "));
            int t = storage.threshold;
            Serial.println(t, DEC);
            
        } 
        else if (data == F("threshold eeprom")) 
        {
            Serial.print(F("Threshold stored in EEPROM is: "));
            int t = storage.threshold;
            Serial.println(t, DEC);
            
        } 
        else if (data == F("print config"))
        {
            serialPrintConfiguration(runtimeStorage, storage);
        }
        else 
        {
            Serial.println(F("Unrecognized command!"));
            printRunCmds();
        }
    }
}

///Function for manually test sub-systems
int debugMode() 
{

    //Wait for input 
    for (;;) 
    {
        if (Serial.available() > 0) 
        {
            break;
        }
    }

    
    // read the incoming string:
    String data = Serial.readStringUntil("\n");

    if (data == F("debug off") || data == F("exit")) 
    {
        //Exit debug mode
        Serial.println(F("Exiting debug mode!"));
        return 0;

    } 
    else if (data == F("alarm on")) 
    {
        buzzer.startAlarm();
    } 
    else if (data == F("alarm off")) 
    {
        buzzer.stopAlarm();
    } 
    else if (data == F("indicator on")) 
    {
        indicator.turnOn();
    } 
    else if (data == F("indicator off")) 
    {
        indicator.turnOff();
    }
    else 
    {
        Serial.println(F("Unrecognized command!"));
        printDebugCmds();
    }


	return 1;

}

void ISRHandler() 
{
    return;
}
