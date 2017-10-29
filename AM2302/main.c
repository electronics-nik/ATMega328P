/*
 * AM2302.c
 *
 * Created: 29.10.2017 21:44:20
 * Author : Nikolay
 */ 

#include "../constants.h"
#include "../sys_led.h"
#include "am2302.h"
#include "i2c.h"
#include "lcd.h"


int main(void)
{
	bool flag = true;
	uint16_t humidity = 0;
	uint16_t temperature = 0;
	char buffer[16] = {};

	i2c_init();
	lcd_init();
	am_init();

	led_init;
	i2c_init();
	lcd_init();

	lcd_clear(LIGHT_ON);

	lcd_print_line1("Home  Controller", LIGHT_ON);
	lcd_print_line2("     v 1.0", LIGHT_ON);

	_delay_ms(3000);
	lcd_clear(LIGHT_ON);

	uint8_t i = 0;

    while (true) 
    {
		uint8_t error = am_get_data(&humidity, &temperature);
		if (!error)
		{
			if (flag)
			{
				led_on;
			}
			else
			{
				led_off;
			}

			sprintf(buffer, "Temp: %i.%i%cC", temperature / 10, temperature % 10, (char)223);
			lcd_print_line1(buffer, LIGHT_ON);

			sprintf(buffer, "Humi: %i.%i %%", humidity / 10, humidity % 10);
			lcd_print_line2(buffer, LIGHT_ON);
		}
		else
		{
			lcd_clear(LIGHT_ON);
			lcd_print_line1("Error!", LIGHT_ON);
		}

		
		
		/*
		sprintf(buffer, "%d -> %c", i, (char)i);
		lcd_print_line1(buffer, LIGHT_ON);
		*/

		flag = !flag;
		i++;
		_delay_ms(500);
    }
}

