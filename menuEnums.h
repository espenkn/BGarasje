#ifndef MENU_ENUMS
#define MENU_ENUMS

enum run_menu {
    MENU_PRINT_ON = 1,
    MENU_PRINT_OFF,
    MENU_PLOT_ON,
    MENU_PLOT_OFF,
    MENU_THRESHOLD_GET,
    MENU_THRESHOLD_SET,
    MENU_THRESHOLD_EEPROM,
    MENU_SAVE_CONFIG = 20,
    MENU_PRINT_CONFIG,
    MENU_ENTER_SERVICE = 30,
    
};


enum service_menu {
    MENU_SERVICE_EXIT = 0,
    MENU_SERVICE_ALARM_ON = 1,
    MENU_SERVICE_ALARM_OFF,
    MENU_SERVICE_INDICATOR_ON,
    MENU_SERVICE_INDICATOR_OFF,
    MENU_SERVICE_CLEAR_EEPROM = 99

};


#endif