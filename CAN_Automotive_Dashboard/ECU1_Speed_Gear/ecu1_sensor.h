
#ifndef ECU1_SENSOR_H
#define	ECU1_SENSOR_H

#include <stdint.h>
#include "digital_keypad.h"


#define MAX_GEAR 5
#define SPEED_ADC_CHANNEL 0x04
#define GEAR_UP             SWITCH1
#define GEAR_DOWN           SWITCH2
#define NEUTRAL             SWITCH3
#define REVERSE             SWITCH4
#define REVERSE_GEAR        0xFF
uint16_t get_speed();
unsigned char get_gear_pos();

#endif	/* ECU1_SENSOR_H */

