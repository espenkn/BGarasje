#ifndef BUZZER
#define BUZZER

#include "Arduino.h"

#define BUZZER_ENA 1


class Buzzer
{
  protected: 
    int pin; 
    bool enabled = true;
    bool valid = false;
    
  public: 
   

    Buzzer(int pin);

    int getPin();
    
    void startAlarm();
    void stopAlarm();
    
    void enable();
    void disable();
    
    bool isEnabled();
    
  protected:
    void registerPin();
    bool isValid();


};

#endif
