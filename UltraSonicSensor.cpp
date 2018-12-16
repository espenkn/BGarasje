#include "UltraSonicSensor.h"
#include "Arduino.h"

UltraSonicSensor::UltraSonicSensor(int trigPin, int echoPin)
{
    this->trigPin = trigPin;
    this->echoPin = echoPin;

    this->registerPins();
}

void UltraSonicSensor::start() {
    if(!this->isValid())
    {
        return;
    }
    // Clears the trigPin
    digitalWrite(this->trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(this->trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->trigPin, LOW);
} 

int UltraSonicSensor::measureTime() {
    /*
    if(!this->isValid())
    {
        return;
    }
    */
    this->start();

    // Reads the echoPin, returns the sound wave travel time in microseconds
    return pulseIn(echoPin, HIGH);

     
}
int UltraSonicSensor::measureDistance() {
   
    // Calculating the distance
    //return (this->measureTime() * this->speedOfSoundInAir) / 2; //full time is "back and forth", half time is "time to target".
    return (this->measureTime() * this->speedOfSoundInAir) / 2000; //full time is "back and forth", half time is "time to target".

}

int UltraSonicSensor::getTrigPin() {
    return this->trigPin;
}
int UltraSonicSensor::getEchoPin() {
    return this->echoPin;
}
void UltraSonicSensor::registerPins()
{
    // if no buttons connected always return 0
    if (ULTRA_SONIC_SENSOR_ENA == 0)
        return;
    
    pinMode(this->trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(this->echoPin, INPUT);  // Sets the echoPin as an Input

    this->valid = true;
}

bool UltraSonicSensor::isValid() 
{
    return this->valid;
}