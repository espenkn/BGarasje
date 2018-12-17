#ifndef PARKING_CONTROL
#define PARKING_CONTROL

#include "Arduino.h"
#include "UltraSonicSensor.h"


class ParkingControl
{
  protected: 
    enum State {
      IDLE,
      START, 
      SEARCHING,
      CAR_DETECTED, 
      CAR_PARKED, 
      CAR_REMOVED, 
      SERVICE, 
      MENU
    };

    UltraSonicSensor* sensor;
    State state = IDLE;
    
  public: 
    

  

    ParkingControl(UltraSonicSensor* sensor);
    
    void iterateStatemachine();
    
    /*
    void checkForCar();
    void signalOn();
    bool timerExpired();
    void monitorParked();
    bool carParked();
    bool carDetected();
    */
      


};

#endif