#include "ParkingControl.h"


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
            //Go to correct state
            if(carDetected()) 
            {
                state = CAR_DETECTED;
            }
            break;
        
        case ParkingControl::CAR_DETECTED:
            
            break;

        default:
            break;
    }
    
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

bool ParkingControl::carDetected() 
{


}

