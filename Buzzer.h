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
    bool printText = true;
    
  public: 
   

    Buzzer(int pin);

    int getPin();
    
    void startAlarm();
    void stopAlarm();
    
    void enable();
    void disable();
    
    bool isEnabled();

    void setPrint(bool onOff);
    
  protected:
    void registerPin();
    bool isValid();
    bool printEnabled();


};

#endif
