#include "ParkingControl.h"


ParkingControl::ParkingControl(UltraSonicSensor* sensor) 
{
    this->sensor = sensor;
}

void ParkingControl::checkForCar() 
{

}

void ParkingControl::signalOn() 
{
    
    //playAlarm();
    ///indicator(true); 
}

bool ParkingControl::timerExpired() 
{

}

void ParkingControl::monitorParked() 
{
    //while(carParked() && !timerExpired()){}
}

bool ParkingControl::carParked() 
{
    //return (threshold < distance);
}