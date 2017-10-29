/*
 * am2302.h
 *
 * Created: 29.10.2017 21:45:02
 *  Author: Nikolay
 */ 


#ifndef AM2302_H_
#define AM2302_H_

#include "../constants.h"

#define DDR_SENSOR   DDRC
#define PORT_SENSOR  PORTC
#define PIN_SENSOR   PINC
#define SENSOR       PC0

void am_init(void);
uint8_t am_get_data(uint16_t *humidity, uint16_t *temp);

#endif /* AM2302_H_ */