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
    int duration = 2000; //2 sec (higher is better but annoying when testing)
    int frequency = 800;
    
  public: 
   

    Buzzer(int pin);

    int getPin();
    
    void startAlarm();
    void stopAlarm();
    
    void enable();
    void disable();

    void enable(bool onOff);
    
    bool isEnabled();

    void setPrint(bool onOff);

    void setFreqency(int freq);
    void setDuration(int dur);
    
  protected:
    void registerPin();
    bool isValid();
    bool printEnabled();


};

#endif
