/*
* Ultrasonic Sensor HC-SR04 
*
* based on code from Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/

// defines pins numbers
const int interuptPin = 2;
const int trigPin = 4;
const int echoPin = 5;
const int buzzerPin = 9;
const int indicatorPin = 10;


//debug flag
bool printMesurments = false;

// defines variables
long duration;
int distance;

//Sate of car
enum carState {in, out};
enum carState currentState;

//distance in cm to define a car is in place (less-than threshold)
const int threshold = 200;

//For timer 
unsigned long parkTime = 0;

/*
char* serialCMDs[] = {
    "debug on",
    "debug off",
    "set threshold",
};

char* debugCMDs[] = {
    "alarm stop",
    "alarm start",
    "print on",
    "print off",
    "indicator on",
    "indicator off"
};
*/
//Prototypes
void ISRHandler(void);



void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    
    pinMode(buzzerPin, OUTPUT); 
    pinMode(indicatorPin, OUTPUT); 
    
    pinMode(interuptPin, INPUT_PULLUP); 

    
    Serial.begin(9600); // Starts the serial communication

    //noInterrupts();
    interrupts();
    attachInterrupt(digitalPinToInterrupt(interuptPin), ISRHandler, LOW);
}

void loop() {
   
    handleConsole();
    
    checkForCar();

    
    

    delay(500);
    
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

    }


}


void setThresholdDistance(int threshold) {
   
}


bool carParked() {
    return (threshold < distance);
}


//Bedre navn
void displayParked() {
    //do something with some pins
    //Blink lights
    //sound the alarm

    //start timer - Run for at least 10 seconds
    //stop timer if car no longer parked
    playAlarm();


    
  
}

void monitorParked() {
    while(carParked() && !timerExpired()){}
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
            Serial.println(F("CMDS:"));
            Serial.println(F("alarm stop|start"));
            Serial.println(F("indicator on|off"));
            Serial.println();

            //Hand over execution to debug mode
            while(debugMode()){};
            
        } else if (data == F("print on")) {
            printMesurments = true;
        } else if (data == F("print off")) {
            printMesurments = false;
        } else if (data == F("set threshold")) {
            Serial.print(F("Enter threshold value in cm: "));
            String newThresholdString = Serial.readStringUntil("\n");
            int newThreshold =  atoi( newThresholdString.c_str() );
        }

        
		
    }


}

///Function for manually test sub-systems
int debugMode() {

    //Wait for input or loss of Serial
    for(;;) {
        if (!Serial) {
            return;
        }
        if (Serial.available() > 0) {
            break;
        }
    }

    
    // read the incoming string:
    String data = Serial.readStringUntil("\n");

    if (data == F("debug off")) {
        //Exit debug mode
        return 0;

    } else if (data == F("alarm sart")) {
        playAlarm();
    } else if (data == F("alarm stop")) {
        stopAlarm();
    } else if (data == F("indicator on")) {
        indicator(true);
    } else if (data == F("indicator off")) {
        indicator(false);
    }


	return 1;

}

void playAlarm() {
    tone(buzzerPin, 800, 10000);
}
void stopAlarm() {
    noTone(buzzerPin);
}

void indicator(bool on) {
    if (on) {
        digitalWrite(indicatorPin, HIGH);
    } else {
        digitalWrite(indicatorPin, LOW);
    }
    
}



void ISRHandler() {
    return;
}