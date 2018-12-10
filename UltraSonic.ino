/*
* Ultrasonic Sensor HC-SR04 
*
* based on code from Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/

#include <EEPROM.h>

// defines pins numbers
const int trigPin = 4;
const int echoPin = 6;
const int buzzerPin = 9;
const int indicatorPin = 11;

// Interupt not implemented!!
const int interuptPin = 2;

//debug flag
bool printMesurments = false;

// defines variables
long duration;
int distance;

//Sate of car
enum carState {in, out};
enum carState currentState;


enum systemState {idle, carOut, carIn, carParking, carLeaving};

//distance in cm to define a car is in place (less-than threshold)
// loaded from EEPROM
int threshold = -1;
const int defaultThreshold = 200;

//For timer 
unsigned long parkTime = 0;


void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    
    pinMode(buzzerPin, OUTPUT); 
    pinMode(indicatorPin, OUTPUT); 
    
    
    Serial.begin(9600); // Starts the serial communication

    /*
    pinMode(interuptPin, INPUT_PULLUP);
    //noInterrupts();
    interrupts();
    attachInterrupt(digitalPinToInterrupt(interuptPin), ISRHandler, LOW);
    */

    threshold = getStoredThresholdDistance();

    if (threshold < 1) {
        //probably bad config, load default
        threshold = defaultThreshold;
    }

}

void loop() {
   
    handleConsole();
    
    checkForCar();

    delay(500);
    
}



void statemachine() {

}


bool timerExpired() {

}


void checkForCar() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = (duration*0.034)/2;

   

    if (carParked()) {
        //possible car in place

        //Store time parked
        parkTime = millis();

    } else {
        //Clear all
    }

}



//Bedre navn
void displayParked() {
    //do something with some pins
    //Blink lights
    //sound the alarm

    //start timer - Run for at least 10 seconds
    //stop timer if car no longer parked
    playAlarm();

    indicator(true);


    
  
}

void monitorParked() {
    while(carParked() && !timerExpired()){}
}




bool carParked() {
    return (threshold < distance);
}


void handleConsole() {
   // Prints the distance on the Serial Monitor
    if (printMesurments && Serial) {
        Serial.print("Distance: ");
        Serial.println(distance);
    }


    // reply only when you receive data:
	if (Serial && Serial.available() > 0) {
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
        if (!Serial) {
            return 0;
        }
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

    } else if (data == F("alarm start")) {
        playAlarm();
    } else if (data == F("alarm stop")) {
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

void printDebugCmds() {
    if (Serial) {
        Serial.println(F("CMDS:"));
        Serial.println(F("alarm stop|start"));
        Serial.println(F("indicator on|off"));
        Serial.println(F("debug off / exit")); //Or exit

        Serial.println();
    }
}

void printRunCmds() {
    if (Serial) {
        Serial.println(F("CMDS:"));
        Serial.println(F("debug on|off"));
        Serial.println(F("print on|off"));
        Serial.println(F("threshold set|get|store|eeprom"));
        Serial.println();
    }
}



void ISRHandler() {
    return;
}
