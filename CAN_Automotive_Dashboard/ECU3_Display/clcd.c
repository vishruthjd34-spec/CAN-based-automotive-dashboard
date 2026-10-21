#include <xc.h>
#define _XTAL_FREQ 20000000UL
#include "clcd.h"
static void pulse(void)
{
    EN = 1;
    __delay_us(2);
    EN = 0;
    __delay_us(100);
}
static void write_byte(unsigned char v, unsigned char rs)
{
    CLCD_DATA_PORT = v;
    RS = rs;
    RW = 0;
    pulse();
}
void init_clcd(void)
{
    TRISD = 0;
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISEbits.TRISE2 = 0;
    __delay_ms(20);
    write_byte(0x38, 0);
    write_byte(0x0C, 0);
    write_byte(0x01, 0);
    __delay_ms(2);
    write_byte(0x06, 0);
}
void clcd_putch(unsigned char c, unsigned char a)
{
    write_byte(a, 0);
    write_byte(c, 1);
}
void clcd_print(const unsigned char *s, unsigned char a)
{
    write_byte(a, 0);
    while (*s)
        write_byte(*s++, 1);
}
