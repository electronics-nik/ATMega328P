/*
 * am2302.c
 *
 * Created: 29.10.2017 21:44:47
 *  Author: Nikolay
 */ 


 #include "am2302.h"
 #include "../pins.h"

#define AM_SENSOR_sda_out			pin_mode_output(AM_DDR, AM_SENSOR)
#define AM_SENSOR_sda_in			AM_DDR &= ~(1 << AM_SENSOR) // release sda => hi in consequence of pullup
#define AM_SENSOR_sda_low			AM_PORT &= ~(1 << AM_SENSOR)
#define AM_SENSOR_is_hi				pin_read(AM_PIN, AM_SENSOR)
#define AM_SENSOR_is_low			!AM_SENSOR_is_hi

#define AM_BUS_NOT_FREE				1
#define CHECKSUM_IS_NOT_CORRECT		7
  
/**
 * @brief init avr for am2302 sda
 */
void am_init(void)
{
	pin_mode_input(AM_PORT, AM_DDR, AM_SENSOR);
}
 
uint8_t am_get_data(uint16_t *humidity, uint16_t *temperature)
{
	uint8_t bits[5]={};
	uint8_t timeout = 200;

	if (AM_SENSOR_is_low)
	{
		return AM_BUS_NOT_FREE;
	}
 
	AM_SENSOR_sda_out;
	AM_SENSOR_sda_low;	// MCU start signal
	_delay_ms(20);	// start signal (pull sda down for min 0.8ms and maximum 20ms)
	AM_SENSOR_sda_in;
 
	// Bus master has released time min: 20us, typ: 30us, max: 200us
	while(AM_SENSOR_is_hi)
	{
		_delay_us(1);
		if (!timeout--)
		{
			return 2;
		}
	}
 
	// AM2302 response signal min: 75us typ:80us max:85us
	timeout = 85;
	while(AM_SENSOR_is_low)
	{
		_delay_us(1);
		if (!timeout--)
		{
			return 3;
		}
	}  // response to low time
 
	timeout = 85;
	while(AM_SENSOR_is_hi)
	{
		_delay_us(1);
		if (!timeout--)
		{
			return 4;
		}
	}  // response to high time
 
 
	/*
	 *			  time in us: min typ max
	 *    signal 0 high time: 22  26  30     (bit=0)
	 *    signal 1 high time: 68  70  75     (bit=1)
	 *  signal 0,1 down time: 48  50  55
	 */
 
	
 
	for(uint8_t i = 0; i < 5; i++)
	{
		uint8_t sensor_byte = 0;
	
		// get 8 bits from sensor
		for(uint8_t j = 1; j <= 8; j++)
		{
			// wait for sensor response
			timeout = 55;
			while(AM_SENSOR_is_low)
			{
				_delay_us(1);
 
				// if timeout == 0 => sensor do not response
				if (!timeout--)
				{
					return 5;
				}
			}
 
			// wait 30 us to check if bit is logical "1" or "0"
			_delay_us(30);
			sensor_byte <<= 1; // add new lower bit
 
			// If sda ist high after 30 us then bit is logical "1" else it was a logical "0"
			// For a logical "1" sda have to be low after 75 us.
			if (AM_SENSOR_is_hi)
			{
				sensor_byte |= 1; // add logical "1"
				timeout = 45;  // 30us - 75us = 45us
 
				while(AM_SENSOR_is_hi)
				{
					_delay_us(1);
				
					if (!timeout--)
					{
						return 6;
					}
				}
			}
		}
 
		bits[i] = sensor_byte;
	}
 
	// checksum
	if ( ((bits[0] + bits[1] + bits[2] + bits[3]) & 0xff ) != bits[4])
	{
		// debug output
		//printf("%b %b %b %b %b %b" CR, sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], sensor_data[4], ((sensor_data[0]+sensor_data[1]+sensor_data[2]+sensor_data[3]) & 0xff ));
		return CHECKSUM_IS_NOT_CORRECT;
	}
 
	*humidity = (bits[0] << 8) + bits[1];
	*temperature = (bits[2] << 8) + bits[3];
 
	return 0;
}