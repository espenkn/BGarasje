#include "Indicator.h"
#include "Arduino.h"



Indicator::Indicator(int pin)
{
    this->pin = pin;
    this->registerPin();
}

void Indicator::turnOn() 
{    
    if (!this->isValid())
    {
        return;
    }

    if (this->isEnabled() == false) 
    {    
        Serial.println(F("Indicator disabled"));
        Serial.flush();
    
        return;
    } 
        
    digitalWrite(this->getPin(), HIGH);
    
    
    Serial.println(F("Indicator on"));
    Serial.flush();
}

void Indicator::turnOff() 
{
    if (!this->isValid())
    {
        return;
    }
    
    digitalWrite(this->getPin(), LOW);
    
    Serial.println(F("Indicator off"));
    Serial.flush();
    
}
    

    
void Indicator::enable() 
{
    this->enabled = true;
}

void Indicator::disable() 
{
    this->enabled = false;
}
    

int Indicator::getPin()
{
    return this->pin;
}

bool Indicator::isEnabled() 
{
    return this->enabled;
}

void Indicator::registerPin()
{

    if (INDICATOR_ENA == 0)
        return;
    
    pinMode(this->getPin(), OUTPUT); 
    
    this->valid = true;
    
}

bool Indicator::isValid() 
{
    return this->valid;
}
