#include "Buzzer.h"
#include "Arduino.h"



Buzzer::Buzzer(int pin)
{
    this->pin = pin;
    this->registerPin();
}

void Buzzer::startAlarm() 
{
    if (!this->isValid())
    {
        return;
    }
    
    if (this->isEnabled() == false) 
    {    
        Serial.println(F("Alarm disabled"));
        Serial.flush();
    
        return;
    } 

    tone(this->getPin(), 800, 10000);

    Serial.println(F("Starting alarm"));
    Serial.flush();  

}

void Buzzer::stopAlarm() 
{
    if (!this->isValid())
    {
        return;
    }

    noTone(this->getPin());
    
    Serial.println(F("Alarm force stoped"));
    Serial.flush();
    
}
    

    
void Buzzer::enable() 
{    
    this->enabled = true;
}

void Buzzer::disable() 
{
    this->enabled = false;
}

bool Buzzer::isEnabled() 
{
    return this->enabled;
}
    

int Buzzer::getPin()
{
    return this->pin;
}

void Buzzer::registerPin()
{
    // if no buttons connected always return 0
    if (BUZZER_ENA == 0)
        return;
    
    pinMode(this->getPin(), OUTPUT); 
    
    this->valid = true;
    
}

bool Buzzer::isValid() 
{
    return this->valid;
}