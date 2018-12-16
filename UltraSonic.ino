#include <EEPROM.h>
#include "support.h"
#include "eepromstore.h"
#include "UltraSonicSensor.h"
#include "Buzzer.h"
#include "Indicator.h"
#include "serialsupport.h"


//debug flag
bool printMesurments = false;

UltraSonicSensor sensor(pinning.TRIG_PIN, pinning.ECHO_PIN);
Indicator indicator(pinning.INDICATOR_PIN);
Buzzer buzzer(pinning.BUZZER_PIN);
//ParkingControl control(sensor);

struct eepromStore storage;
struct eepromStore runtimeStorage;

void setup() {

    // Sett IDLE state
    state = START;

    // Restore EEPROM storage
    restoreStorage(storage);

    
    Serial.begin(31250); // Starts the serial communication

    // Copy storage 
    runtimeStorage = storage;

    // Handle bad configuration....
    if (runtimeStorage.threshold < 1) 
    { 
        Serial.println(F("Bad sensor calibartion. Loading default. Please run calibrate and store to eeprom.")); 
        runtimeStorage.threshold = defaultThreshold;
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
    iterateStatemachine();


    handleConsole();
    
    checkForCar();

    delay(500);
    
}

/*
 CAR_DETECTED, 
    CAR_PARKED, 
    CAR_REMOVED, 
    SEARCHING
    SERVICE, 
    MENU*/

void iterateStatemachine()
{
  
    switch (state) 
    {
        case IDLE:
        case SEARCHING:
            //Go to correct state
            if(control.carDetected()) 
            {
                state = CAR_DETECTED;
            }
            break;
        
        case CAR_DETECTED:
            
            break;

        default:
            break;
    }

}


void handleConsole() 
{
    
    
    /*
    // Prints the distance on the Serial Monitor
    if (printMesurments) {
        Serial.print("Distance: ");
        Serial.println(distance);
    }
    */

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
            printMesurments = true;

        } 
        else if (data == F("print off")) 
        {
            printMesurments = false;

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
            
            setThresholdDistance(newThreshold);

            Serial.println();
            Serial.print("Got value: ");
            Serial.println(newThreshold, DEC);
            Serial.println("To SAVE to eeprom run \"threshold store\"");
            

        } 
        else if (data == F("threshold get")) 
        {
            Serial.print(F("Threshold value in cm: "));
            Serial.println(threshold, DEC);
            
        } 
        else if (data == F("threshold store"))
        {
            storeThresholdDistance();
            Serial.println(F("Stored to EEPROM"));

        } 
        else if (data == F("threshold eeprom")) 
        {
            Serial.print(F("Threshold stored in EEPROM is: "));
            int t = getStoredThresholdDistance();
            Serial.println(t, DEC);
            
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
        buzzer.playAlarm();
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
