/*
Name       : Shrikrishna Ranbhare
Date        : 11-09-2026

Project     : CAN BASED AUTOMOTIVE DASHBOARD (ECU1).

Description :  Implemented ECU1 of an automotive embedded system using the CAN protocol for inter-ECU communication. 
               The system reads vehicle speed through the ADC module and detects gear position using a digital keypad. 
               The processed data is periodically transmitted over the CAN bus to the display ECU for real-time monitoring.
 */
#include <xc.h>
#define _XTAL_FREQ 20000000
#include "adc.h"
#include "can.h"
#include "ecu1_sensor.h"
#include "msg_id.h"
#include "digital_keypad.h"
static void init_config(void)
{
    init_adc();
    init_digital_keypad();
    init_can();
}
int main(void)
{
    uint8_t speed, gear;
    init_config();
    while (1)
    {
        speed = (uint8_t)(((uint32_t)get_speed() * 100) / 1023);
        gear = get_gear_pos();
        can_transmit(SPEED_MSG_ID, &speed, 1);
        __delay_ms(10);
        can_transmit(GEAR_MSG_ID, &gear, 1);
        __delay_ms(10);
    }
}
