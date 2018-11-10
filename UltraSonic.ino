/*
* Ultrasonic Sensor HC-SR04 
*
* based on code from Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/

// defines pins numbers
const int trigPin = 9;
const int echoPin = 10;

// defines variables
long duration;
int distance;



//distance in cm to define a car is in place (less than threshold)
const int threshold = 200;







void setup() {
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
}

void loop() {
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
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);


    delay(500);
    
}


void setThresholdDistance(int threshold) {
   
}


boolean carParked() {
  return (threshold < distance);
}


//Bedre navn
void displayParked() {
  //do something with some pins
  //Blink lights
  //sound the alarm

  //start timer - Run for at least 10 seconds
  //stop timer if car no longer parked


 // while(carParked() && !timerExpired()){}
  

  
}













