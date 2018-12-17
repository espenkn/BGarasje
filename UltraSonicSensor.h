#ifndef ULTRA_SONIC_SENSOR
#define ULTRA_SONIC_SENSOR

#include "Arduino.h"

#define ULTRA_SONIC_SENSOR_ENA 1


class UltraSonicSensor
{
  protected: 
    int trigPin; //lage const?
    int echoPin; //lage const?
    bool valid;

    bool print = false;
    
  public: 
   

    //static const float speedOfSoundInAir = 0.034;
    static const int speedOfSoundInAir = 34;
    
    UltraSonicSensor(int trigPin, int echoPin);

    int getTrigPin();
    int getEchoPin();

    long int measureTime();
    int measureDistance();

    //for use with serial monitor
    void setPrint(bool onOff);
    

  protected:
    
    void registerPins();
    void start();
    bool isValid();

    bool printEnabled(); 
};

#endif
