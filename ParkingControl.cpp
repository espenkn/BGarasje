#include "ParkingControl.h"
#include "UltraSonicSensor.h"


ParkingControl::ParkingControl(UltraSonicSensor* sensor) 
{
    this->sensor = sensor;
}


bool ParkingControl::searchForCar()
{
   
    int distance = movingAverageMesurments(sensor->measureDistance());

    if (printEnabled()) 
    {
        Serial.println(distance, DEC);
    }

    /*
    Algorithms:

    hard threshold:
        - Distance to ground shorter than threshold
    smart threshold:
        - Distance to ground shorter than threshold or distance longer than actual distance. 
        - If distance is longer than than typicaly means that a object do not reflect echo back to sensor.

    deviation in percent:
        - Disturbance in percent from threshold (either way)
        
    deviation in cm:
        - Disturbance in cm from threshold (either way)

    */

   switch (activeAlgorith) 
   {
        case ALGO_THRESHOLD_BASIC:
            return (threshold < distance);
            break;
        
        case ALGO_THRESHOLD_BASIC_MOVING:
            return (threshold < movingAverageMesurments(distance));
            break;

        case ALGO_THRESHOLD_SMART:
            return (threshold < distance || distance > normalDistance);
            break;
        
        case ALGO_DEVIATION_PERCENT:
            
        
        case ALGO_DEVIATION_FIXED:
            

        default:
            //Bad algo
            //Always return false
            return false;
   }

    return false;

}

int ParkingControl::movingAverageMesurments(int mesurment)
{

    //Serial.println(mesurment, DEC);


    ///Naive implementation
    //shift all
    int curAvg = 0;
    
    for (int i = 0; i < MOVING_AVG_BUF_SIZE-1; i++)
    {
        avg[i] = avg[i+1];
        curAvg += avg[i+1];
    }

    avg[MOVING_AVG_BUF_SIZE-1] = mesurment;
    curAvg += mesurment;
    curAvg /= MOVING_AVG_BUF_SIZE;

    return curAvg;
}

void ParkingControl::iterateStatemachine()
{
    
    bool detected;

    switch (state)
    {
        case IDLE:
            ///Prepare system. Start sensor and discard first 100 readings (loads up moving average)
            for(int i = 0; i < 100; i++)
            {
                movingAverageMesurments(sensor->measureDistance());
            }

            // System active. 
            state = SEARCHING;
            break;

        case SEARCHING:

            //Serial.println("STATE: SEARCHING");

            ///Run Sensor. 
            // IF sensor deviates, something is detected
            detected = searchForCar();
            if (detected) 
            {
                //state = CAR_DETECTED;
                //Serial.println("Car detected. State not changed");
            }


            break;
        
        case CAR_DETECTED:
            ///Filter and determain if this is actualy what we are looking for
            Serial.println("STATE: CAR_DETECTED");

            

            break;

        case CAR_PARKED:
            // if sensor deviates, the car change state to CAR_REMOVED
            Serial.println("STATE: CAR_PARKED");
            break;

        case CAR_REMOVED:
            //Reset logic

            Serial.println("STATE: CAR_REMOVED");
            state = SEARCHING;

            break;

        default:
            break;
    }
    
}

int ParkingControl::algoThresholdBasic(int measurment)
{
    
}

void ParkingControl::setThreshold(int threshold)
{
    this->threshold = threshold;
}

void ParkingControl::changeAlgorithm(int algo)
{
    activeAlgorith = algo;
}


void ParkingControl::setPrint(bool onOff) 
{
    this->print = onOff;
}

bool ParkingControl::printEnabled() 
{
    return this->print;
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
