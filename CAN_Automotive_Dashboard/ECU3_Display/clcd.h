#ifndef CLCD_H
#define CLCD_H
#include <xc.h>
#define CLCD_DATA_PORT LATD
#define RS LATCbits.LATC0
#define RW LATCbits.LATC1
#define EN LATEbits.LATE2
#define INST_MODE 0
#define DATA_MODE 1
void init_clcd(void);
void clcd_print(const unsigned char *, unsigned char);
void clcd_putch(unsigned char, unsigned char);
#endif
