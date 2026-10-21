#include <xc.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "message_handler.h"
static uint8_t speed, gear, temp, status;
static uint16_t rpm;
static void format_num(uint16_t n, unsigned char *b, uint8_t w)
{
    b[w] = 0;
    while (w--)
    {
        b[w] = (n % 10) + '0';
        n /= 10;
    }
}
void process_canbus_data(void)
{
    uint16_t id;
    uint8_t d[8], l = 0;
    can_receive(&id, d, &l);
    if (!l)
        return;
    switch (id)
    {
    case SPEED_MSG_ID:
        speed = d[0];
        break;
    case GEAR_MSG_ID:
        gear = d[0];
        break;
    case RPM_MSG_ID:
        if (l >= 2)
            rpm = (uint16_t)d[0] | ((uint16_t)d[1] << 8);
        break;
    case ENG_TEMP_MSG_ID:
        temp = d[0];
        break;
    case INDICATOR_MSG_ID:
        status = d[0];
        break;
    default:
        return;
    }
    display_dashboard();
}
void display_dashboard(void)
{
    unsigned char b[6];
    clcd_print((const unsigned char *)"SPD GR RPM TEMP", 0x80);
    format_num(speed, b, 3);
    clcd_print(b, 0xC0);
    if (gear == 0xFF)
        clcd_print((const unsigned char *)"R", 0xC4);
    else if (gear == 0)
        clcd_print((const unsigned char *)"N", 0xC4);
    else
    {
        clcd_putch('G', 0xC4);
        clcd_putch('0' + gear, 0xC5);
    }
    format_num(rpm, b, 4);
    clcd_print(b, 0xC7);
    format_num(temp, b, 3);
    clcd_print(b, 0xCD); /* indicators are driven on RB0/RB1 left, RB6/RB7 right */
    if (status == 1)
    {
        LATB = (LATB & ~0xC0) | 0x03;
    }
    else if (status == 2)
    {
        LATB = (LATB & ~0x03) | 0xC0;
    }
    else
    {
        LATB &= ~0xC3;
    }
}
