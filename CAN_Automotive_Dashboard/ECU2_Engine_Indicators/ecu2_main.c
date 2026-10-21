/*
 Name       : Shrikrishna Ranbhare

Date        : 11-09-2026

Project     : CAN BASED AUTOMOTIVE DASHBOARD (ECU2).

Description :  Implemented ECU2 of an automotive embedded system to monitor engine and vehicle parameters using the CAN protocol. 
               The system reads engine RPM and temperature through the ADC module and processes indicator status using a digital 
               input interface. The processed RPM, temperature, and indicator data are periodically transmitted over the CAN bus 
               to the display ECU for real-time monitoring.
 */
#include <xc.h>
#define _XTAL_FREQ 20000000UL
#include "adc.h"
#include "can.h"
#include "ecu2_sensor.h"
#include "digital_keypad.h"
#include "msg_id.h"
static void init_config(void)
{
    init_adc();
    init_digital_keypad();
    init_can();
}
int main(void)
{
    uint16_t rpm;
    uint8_t temp, ind, data[2];
    init_config();
    while (1)
    {
        rpm = (uint16_t)(((uint32_t)get_rpm() * 6000) / 1023);
        data[0] = (uint8_t)(rpm & 0xFF);
        data[1] = (uint8_t)(rpm >> 8);
        can_transmit(RPM_MSG_ID, data, 2);
        __delay_ms(10);
        ind = (uint8_t)process_indicator();
        can_transmit(INDICATOR_MSG_ID, &ind, 1);
        __delay_ms(10);
        temp = (uint8_t)(((uint32_t)read_adc(ENG_TEMP_ADC_CHANNEL) * 100) / 1023);
        can_transmit(ENG_TEMP_MSG_ID, &temp, 1);
        __delay_ms(10);
    }
}
