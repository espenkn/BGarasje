#ifndef PARKING_CONTROL
#define PARKING_CONTROL

#include "Arduino.h"
#include "UltraSonicSensor.h"


class ParkingControl
{
  protected: 
    UltraSonicSensor* sensor;
    
  public: 
    enum States {
        IDLE,
        START, 
        SEARCHING,
        CAR_DETECTED, 
        CAR_PARKED, 
        CAR_REMOVED, 
        SERVICE, 
        MENU
    };

  

    ParkingControl(UltraSonicSensor* sensor);
    
    void checkForCar();
    void signalOn();
    bool timerExpired();
    void monitorParked();
    bool carParked();

};

#endif