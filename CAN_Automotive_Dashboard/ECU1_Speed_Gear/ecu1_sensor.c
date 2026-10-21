
 #include "ecu1_sensor.h"
 #include "adc.h"
 #include "can.h"
 #include "digital_keypad.h"
 
 
 uint16_t get_speed()
 {
     return read_adc(CHANNEL4);
 }
 
 unsigned char get_gear_pos()
 {
     static unsigned char gear = 0;
     unsigned char key = read_digital_keypad(STATE_CHANGE);
 
     switch(key)
     {
         case GEAR_UP:
             if(gear != REVERSE_GEAR && gear < MAX_GEAR)
                 gear++;
             break;
 
         case GEAR_DOWN:
             if(gear && gear != REVERSE_GEAR)
                 gear--;
             break;
 
         case NEUTRAL:
             gear = 0;
             break;
 
         case REVERSE:
             gear = REVERSE_GEAR;
             break;
     }
 
     return gear;
 }
 
 