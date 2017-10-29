/*
 * am2302.c
 *
 * Created: 29.10.2017 21:44:47
 *  Author: Nikolay
 */ 


 #include "am2302.h"

#define SENSOR_sda_out		DDR_SENSOR |= (1 << SENSOR)
#define SENSOR_sda_in		DDR_SENSOR &= ~(1 << SENSOR) // release sda => hi in consequence of pullup
#define SENSOR_sda_low		PORT_SENSOR &= ~(1 << SENSOR)
#define SENSOR_is_hi		PIN_SENSOR & (1 << SENSOR)
#define SENSOR_is_low		!(PIN_SENSOR & (1 << SENSOR))
  
/**
 * @brief init avr for am2302 sda
 */
void am_init(void)
{
	DDR_SENSOR &= ~(1 << SENSOR); // define as input
	PORT_SENSOR &= ~(1 << SENSOR);  // disable pullup
}
 
uint8_t am_get_data(uint16_t *humidity, uint16_t *temp)
{
	if (SENSOR_is_low)
	{
		// bus not free
		return 1;
	}
 
	SENSOR_sda_out;
	SENSOR_sda_low;	// MCU start signal
	_delay_ms(20);	// start signal (pull sda down for min 0.8ms and maximum 20ms)
	SENSOR_sda_in;
 
	// Bus master has released time min: 20us, typ: 30us, max: 200us
	uint8_t timeout = 200;
	while(SENSOR_is_hi)
	{
		_delay_us(1);
		if (!timeout--)
		{
			return 2;
		}
	}
 
	// AM2302 response signal min: 75us typ:80us max:85us
	timeout = 85;
	while(SENSOR_is_low)
	{
		_delay_us(1);
		if (!timeout--)
		{
			return 3;
		}
	}  // response to low time
 
	timeout = 85;
	while(SENSOR_is_hi)
	{
		_delay_us(1);
		if (!timeout--)
		{
			return 4;
		}
	}  // response to high time
 
 
	/*
	 *            time in us: min typ max
	 *    signal 0 high time: 22  26  30     (bit=0)
	 *    signal 1 high time: 68  70  75     (bit=1)
	 *  signal 0,1 down time: 48  50  55
	 */
 
	uint8_t sensor_data[5]={0};
 
	for(uint8_t i = 0; i < 5; i++)
	{
		uint8_t sensor_byte = 0;
	
		// get 8 bits from sensor
		for(uint8_t j = 1; j <= 8; j++)
		{
			// wait for sensor response
			timeout = 55;
			while(SENSOR_is_low)
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
			if (SENSOR_is_hi)
			{
				sensor_byte |= 1; // add logical "1"
				timeout = 45;  // 30us - 75us = 45us
 
				while(SENSOR_is_hi)
				{
					_delay_us(1);
				
					if (!timeout--)
					{
						return 6;
					}
				}
			}
		}
 
		sensor_data[i] = sensor_byte;
	}
 
	// checksum
	if ( ((sensor_data[0] + sensor_data[1] + sensor_data[2] + sensor_data[3]) & 0xff ) != sensor_data[4])
	{
		// debug output
		//printf("%b %b %b %b %b %b" CR, sensor_data[0], sensor_data[1], sensor_data[2], sensor_data[3], sensor_data[4], ((sensor_data[0]+sensor_data[1]+sensor_data[2]+sensor_data[3]) & 0xff ));
		return 7;
	}
 
	*humidity = (sensor_data[0] << 8) + sensor_data[1];
	*temp = (sensor_data[2] << 8) + sensor_data[3];
 
	return 0;
}