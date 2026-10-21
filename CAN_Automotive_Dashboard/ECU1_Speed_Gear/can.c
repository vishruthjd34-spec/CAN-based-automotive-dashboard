#include <xc.h>
#include "can.h"

typedef enum
{
    CAN_NORMAL = 0x00,
    CAN_LOOPBACK = 0x40,
    CAN_CONFIG = 0x80
} CanOpMode;
static void set_mode(CanOpMode mode)
{
    CANCON = (CANCON & 0x1F) | mode;
    while ((CANSTAT & 0xE0) != mode)
        ;
}
static uint16_t get_std_id(void) { return ((uint16_t)RXB0SIDH << 3) | ((RXB0SIDL >> 5) & 0x07); }
static void set_std_id(uint16_t id)
{
    TXB0SIDH = (uint8_t)(id >> 3);
    TXB0SIDL = (uint8_t)((id & 0x07) << 5);
}
void init_can(void)
{
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 1;
    set_mode(CAN_CONFIG);
    ECANCON = 0x00;
    /* Keep all ECUs on exactly the same CAN timing. Existing project timing retained. */
    BRGCON1 = 0xE1;
    BRGCON2 = 0x1B;
    BRGCON3 = 0x03;
    RXM0SIDH = 0;
    RXM0SIDL = 0;
    RXF0SIDH = 0;
    RXF0SIDL = 0;
    RXFCON0 = 0;
    RXB0CON = 0x60; /* receive all valid messages */
    PIR3bits.RXB0IF = 0;
    set_mode(CAN_NORMAL);
}
uint8_t can_transmit(uint16_t msg_id, const uint8_t *data, uint8_t len)
{
    uint8_t i;
    if (!data || len > 8)
        return 0;
    while (TXB0CONbits.TXREQ)
        ;
    TXB0CON = 0;
    TXB0EIDH = 0;
    TXB0EIDL = 0;
    set_std_id(msg_id);
    TXB0DLC = len & 0x0F;
    for (i = 0; i < len; i++)
        ((volatile uint8_t *)&TXB0D0)[i] = data[i];
    TXB0CONbits.TXREQ = 1;
    return 1;
}
void can_receive(uint16_t *msg_id, uint8_t *data, uint8_t *len)
{
    uint8_t i, n;
    if (!msg_id || !data || !len)
        return;
    *len = 0;
    if (!RXB0CONbits.RXFUL)
        return;
    *msg_id = get_std_id();
    n = RXB0DLC & 0x0F;
    if (n > 8)
        n = 8;
    for (i = 0; i < n; i++)
        data[i] = ((volatile uint8_t *)&RXB0D0)[i];
    *len = n;
    RXB0CONbits.RXFUL = 0;
    PIR3bits.RXB0IF = 0;
}
