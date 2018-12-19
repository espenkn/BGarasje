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

        if (this->printEnabled()) 
        {
            Serial.println(F("Alarm disabled"));
            Serial.flush();
        }
        
    
        return;
    } 

    tone(this->getPin(), this->frequency, this->duration);

    if (this->printEnabled()) 
    {
        Serial.println(F("Starting alarm"));
        Serial.flush();  
    }
}

void Buzzer::stopAlarm() 
{
    if (!this->isValid())
    {
        return;
    }

    noTone(this->getPin());
    
    if (this->printEnabled()) 
    {
        Serial.println(F("Alarm force stoped"));
        Serial.flush();
    }
}
    

    
void Buzzer::enable() 
{    
    this->enabled = true;
}

void Buzzer::enable(bool onOff) 
{    
    this->enabled = onOff;
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

void Buzzer::setPrint(bool onOff) 
{
    this->printText = onOff;
}

bool Buzzer::printEnabled() 
{
    return this->printText;
}


void Buzzer::setFreqency(int freq)
{
    this->frequency = freq;
}

void Buzzer::setDuration(int dur)
{
    this->duration = dur;
}