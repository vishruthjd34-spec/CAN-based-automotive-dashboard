#ifndef CAN_H
#define CAN_H
#include <stdint.h>
void init_can(void);
uint8_t can_transmit(uint16_t msg_id, const uint8_t *data, uint8_t len);
void can_receive(uint16_t *msg_id, uint8_t *data, uint8_t *len);
#endif
