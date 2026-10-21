/*
 Name       : Shrikrishna Ranbhare

Date        : 11-09-2026

Project     : CAN BASED AUTOMOTIVE DASHBOARD (ECU3).

Description :  Developed a display ECU to receive and process vehicle parameters transmitted over the CAN bus. 
               The system initializes CAN communication and a 16×2 character LCD, continuously processes incoming 
               CAN messages, and displays the received engine RPM, temperature, and indicator status for real-time 
               vehicle monitoring.
 */
#include <xc.h>
#define _XTAL_FREQ 20000000
#pragma config OSC = HS, WDT = OFF, LVP = OFF, PBADEN = OFF
#include "can.h"
#include "clcd.h"
#include "message_handler.h"
static void init_config(void)
{
    TRISB = 0x08;
    LATB = 0;
    init_clcd();
    init_can();
}
void main(void)
{
    init_config();
    while (1)
    {
        process_canbus_data();
        __delay_ms(20);
    }
}
