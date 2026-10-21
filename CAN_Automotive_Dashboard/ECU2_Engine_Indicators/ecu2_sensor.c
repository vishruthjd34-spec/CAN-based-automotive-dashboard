
#include "ecu2_sensor.h"
#include "adc.h"
#include "digital_keypad.h"

volatile IndicatorStatus cur_ind_status = e_ind_off;

uint16_t get_rpm()
{
    return read_adc(CHANNEL4);
}

IndicatorStatus process_indicator()
{
    unsigned char key = read_digital_keypad(STATE_CHANGE);
    
    switch(key)
    {
        case SWITCH1: 
                cur_ind_status = e_ind_left;  
                break;
                
        case SWITCH2: 
                cur_ind_status = e_ind_right; 
                break;
                
        case SWITCH3: 
                cur_ind_status = e_ind_off;   
                break;
        default:
            break;        
    }
    return cur_ind_status;
}



