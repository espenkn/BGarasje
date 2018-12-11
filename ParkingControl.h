#ifndef PARKING_CONTROL
#define PARKING_CONTROL

#include "Arduino.h"
#include "UltraSonicSensor.h"


class ParkingControl
{
  protected: 
    UltraSonicSensor* sensor;
    
  public: 
   

    ParkingControl(UltraSonicSensor* sensor);
    
    void checkForCar();
    void signalOn();
    bool timerExpired();
    void monitorParked();
    bool carParked();

};

#endif