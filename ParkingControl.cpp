#include "ParkingControl.h"
#include "UltraSonicSensor.h"

ParkingControl::ParkingControl(UltraSonicSensor* sensor) 
{
    this->sensor = sensor;
}


void ParkingControl::iterateStatemachine()
{
    
    switch (state)
    {
        case IDLE:
        case SEARCHING:
            
            break;
        
        case CAR_DETECTED:
            
            break;

        case CAR_PARKED:
        case CAR_REMOVED:
        case SERVICE:
        case MENU:
            break;

        default:
            break;
    }
    
}


/*
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

bool ParkingControl::carDetected() 
{


}
*/
