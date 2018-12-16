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
    bool printText = true;
    
  public: 
   

    Indicator(int pin);

    int getPin();
    
    void turnOn();
    void turnOff();
    
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
