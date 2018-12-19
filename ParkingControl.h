#ifndef PARKING_CONTROL
#define PARKING_CONTROL

#include "Arduino.h"
#include "UltraSonicSensor.h"

#define MOVING_AVG_BUF_SIZE 20

class ParkingControl
{
  public: 
    enum Algorithms {
      ALGO_THRESHOLD_BASIC,
      ALGO_THRESHOLD_BASIC_MOVING,
      ALGO_THRESHOLD_SMART_MOVING,
      ALGO_THRESHOLD_SMART,
      ALGO_DEVIATION_PERCENT,
      ALGO_DEVIATION_FIXED,
      
      // if choise is greater or equal to this => error
      ALGO_LAST_NOT_APPLICABLE 
    };
  
  protected: 
    enum State {
      IDLE,
      START, 
      SEARCHING,
      CAR_DETECTED, 
      CAR_PARKED, 
      CAR_REMOVED
    };

  
  protected: 
    UltraSonicSensor* sensor;
    State state = IDLE;

    int activeAlgorith = ALGO_THRESHOLD_BASIC;

    int threshold = 0;
    
    int normalDistance = 200;   //FAKE THIS VALUE (FOR NOW)
    int headRoom = 100;         //FAKE THIS VALUE (FOR NOW)
    int deviation_cm = 30;      //FAKE THIS VALUE (FOR NOW)
    int deviation_percent = 10; //FAKE THIS VALUE (FOR NOW)

    int badReadingThreshold = normalDistance + headRoom;

    int avg[MOVING_AVG_BUF_SIZE] = {0};

    bool print = false;
  
  
  public: 
    ParkingControl(UltraSonicSensor* sensor);
    
    void iterateStatemachine();

    void printAlgorithms();

    bool searchForCar();
    void setThreshold(int threshold);
    void changeAlgorithm(int algo);

    void setPrint(bool on);
    bool printEnabled();

  protected:
    int algoThresholdBasic(int mesurment);
    int algoThresholdSmart(int mesurment);
    int movingAverageMesurments(int mesurment);

};

#endif