#include <EEPROM.h>
#include "support.h"
#include "eepromstore.h"
#include "UltraSonicSensor.h"
#include "Buzzer.h"
#include "Indicator.h"

/*
// defines pins numbers
const int trigPin = 4;
const int echoPin = 6;
const int buzzerPin = 9;
const int indicatorPin = 11;

// Interupt not implemented!!
const int interuptPin = 2;

const float speedOfSound = 0.034;
*/


//debug flag
bool printMesurments = false;

/*
// defines variables
long duration;
int distance;
*/

/*
//Sate of car
enum carState {in, out};
enum carState currentState;


enum systemState {idle, carOut, carIn, carParking, carLeaving};
*/

//distance in cm to define a car is in place (less-than threshold)
// loaded from EEPROM
/*
int threshold = -1;
const int defaultThreshold = 200;

//For timer 
unsigned long parkTime = 0;


struct config {
    int threshold;
    bool alarmEnabeld;
};
*/

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

    /*
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    
    pinMode(buzzerPin, OUTPUT); 
    pinMode(indicatorPin, OUTPUT); 
    */
    
    Serial.begin(9600); // Starts the serial communication

    /*
    pinMode(interuptPin, INPUT_PULLUP);
    //noInterrupts();
    interrupts();
    attachInterrupt(digitalPinToInterrupt(interuptPin), ISRHandler, LOW);
    */


    // Handle bad configuration....
    if (storage.threshold < 1) { 
        Serial.println(F("Bad sensor calibartion. Please run calibrate")); 
        storage.threshold = defaultThreshold;
    }
    if (storage.alarmEnabeld == 0) { 
        Serial.println(F("Alarm is disabled"));
    }    
    

    // Copy storage after hadle bad config
    runtimeStorage = storage;

    // Print current configuration on serial
    serialPrintConfiguration();


    threshold = getStoredThresholdDistance();

    if (threshold < 1) {
        //probably bad config, load default
        threshold = defaultThreshold;
    }

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
  
    switch (state) {
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


void handleConsole() {
   // Prints the distance on the Serial Monitor
    if (printMesurments) {
        Serial.print("Distance: ");
        Serial.println(distance);
    }


    // reply only when you receive data:
	if (Serial.available() > 0) {
		// read the incoming string:
        String data = Serial.readStringUntil("\n");

        if (data == F("debug on")) {
            Serial.println(F("Entering debug mode!"));
            printDebugCmds();

            //Hand over execution to debug mode
            while(debugMode()){};
            
        } else if (data == F("print on")) {
            printMesurments = true;

        } else if (data == F("print off")) {
            printMesurments = false;

        } else if (data == F("threshold set")) {
            
            Serial.print(F("Enter threshold value in cm: "));
            //String newThresholdString = Serial.readStringUntil("\n");
            //int newThreshold =  atoi( newThresholdString.c_str() );
            while(!Serial.available()){
                delay(20); //wait for input
            }
            int newThreshold = Serial.parseInt();
            
            setThresholdDistance(newThreshold);
            Serial.println();
            Serial.print("Got value: ");
            Serial.println(newThreshold, DEC);
            Serial.println("To SAVE to eeprom run \"threshold store\"");
            

        } else if (data == F("threshold get")) {
            Serial.print(F("Threshold value in cm: "));
            Serial.println(threshold, DEC);
            
        } else if (data == F("threshold store")) {
            storeThresholdDistance();
            Serial.println(F("Stored to EEPROM"));

        } else if (data == F("threshold eeprom")) {
            Serial.print(F("Threshold stored in EEPROM is: "));
            int t = getStoredThresholdDistance();
            Serial.println(t, DEC);
            
        } else {
            Serial.println(F("Unrecognized command!"));
            printRunCmds();
        }

        
		
    }


}

///Function for manually test sub-systems
int debugMode() {

    //Wait for input or loss of Serial
    for(;;) {
        if (Serial.available() > 0) {
            break;
        }
    }

    
    // read the incoming string:
    String data = Serial.readStringUntil("\n");

    if (data == F("debug off") || data == F("exit")) {
        //Exit debug mode
        Serial.println(F("Exiting debug mode!"));
        return 0;

    } else if (data == F("alarm on")) {
        playAlarm();
    } else if (data == F("alarm off")) {
        stopAlarm();
    } else if (data == F("indicator on")) {
        indicator(true);
    } else if (data == F("indicator off")) {
        indicator(false);
    } else {
        Serial.println(F("Unrecognized command!"));
        printDebugCmds();
    }


	return 1;

}
/*
void playAlarm() {
    
    tone(buzzerPin, 800, 10000);
    
    if (Serial) {
        Serial.println(F("Starting alarm"));
    }
}
void stopAlarm() {
    
    noTone(buzzerPin);
    
    if (Serial) {
        Serial.println(F("Alarm force stoped"));
    }
}
*/
/*
void indicator(bool on) {
    if (on) {
        digitalWrite(indicatorPin, HIGH);
        
        if (Serial) {
            Serial.println(F("Indicator on"));
        }
    } else {
        digitalWrite(indicatorPin, LOW);
        
        if (Serial) {
            Serial.println(F("Indicator off"));
        }
    }
    
}
*/
/*
// set runtime threshold
void setThresholdDistance(int newThreshold) {
    threshold = newThreshold;
}

// get runtime threshold
int getThresholdDistance() {
    return threshold;
}
// store runtime threshold to EEPROM
void storeThresholdDistance() {
    EEPROM.put(0, threshold);
}

int getStoredThresholdDistance() {
    int t;
    EEPROM.get(0, t);

    return t;
}
*/

/*
void printDebugCmds() {
    if (Serial) {
        Serial.println(F("CMDS:"));
        Serial.println(F("alarm on|off"));
        Serial.println(F("indicator on|off"));
        Serial.println(F("debug off / exit")); //Or exit

        Serial.println();
        Serial.flush();
    }
}

void printRunCmds() {
    if (Serial) {
        Serial.println(F("CMDS:"));
        Serial.println(F("debug on|off"));
        Serial.println(F("print on|off"));
        Serial.println(F("threshold set|get|store|eeprom"));
        Serial.println();
        Serial.flush();
    }
}

*/

void ISRHandler() {
    return;
}
