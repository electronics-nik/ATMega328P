/*
 * am2302.h
 *
 * Created: 29.10.2017 21:45:02
 *  Author: Nikolay
 */ 


#ifndef AM2302_H_
#define AM2302_H_

#include "../constants.h"

#define AM_DDR		DDRC
#define AM_PORT		PORTC
#define AM_PIN		PINC
#define AM_SENSOR	PC2

void am_init(void);
uint8_t am_get_data(uint16_t *humidity, uint16_t *temperature);

#endif /* AM2302_H_ */