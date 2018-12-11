#ifndef INDICATOR
#define INDICATOR

#include "Arduino.h"

#define INDICATOR_ENA 1


class Indicator
{
  protected: 
    int pin; 
    bool enabled = true;
    bool valid = false;
    
  public: 
   

    Indicator(int pin);

    int getPin();
    
    void turnOn();
    void turnOff();
    
    void enable();
    void disable();
    
    bool isEnabled();

  protected:
    void registerPin();
    bool isValid();


};

#endif
