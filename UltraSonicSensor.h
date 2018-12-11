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
    
  public: 
   

    static const float speedOfSoundInAir = 0.034;
    
    UltraSonicSensor(int trigPin, int echoPin);

    int getTrigPin();
    int getEchoPin();

    int measureTime();
    int measureDistance();
    

  protected:
    void registerPins();
    void start();
    bool isValid();

};

#endif
