#ifndef BB_SUPPORT
#define BB_SUPPORT

#include "Arduino.h"

const int defaultThreshold = 200;

static const struct PinConfig {
    static const char TRIG_PIN = 4;
    static const char ECHO_PIN = 6;
    static const char BUZZER_PIN = 9;
    static const char INDICATOR_PIN = 11;
    //static const char INTERUPT_PIN = 2;

} pinning;


enum parking_system_states {
    START, 
    CAR_DETECTED, 
    CAR_PARKED, 
    CAR_REMOVED, 
    SERVICE, 
    MENU
};
enum parking_system_states state;

enum menu_state {MENU_NONE, MENU_SERIAL, MENU_PANEL};
enum menu_state mstate;

//Sate of car
enum carState {in, out};
enum carState currentState;


enum systemState {idle, carOut, carIn, carParking, carLeaving};


#endif
