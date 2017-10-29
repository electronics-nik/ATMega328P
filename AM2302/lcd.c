/*
 * lcd.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * LCD: 16x2 HD44780 (��� ������)
 *
 * Created: 15.10.2017 22:35:24
 *  Author: Nikolay
 */ 

#include "lcd.h"

char buffer[6];

void push_nibble(uint8_t nibble, uint8_t light_is_on, uint8_t is_command)
{
	(light_is_on ? SET_LIGHT(nibble) : RESET_LIGHT(nibble));
	(is_command ? RESET_RS(nibble) : SET_RS(nibble));

	SET_EN(nibble);
	i2c_write(nibble);
	_delay_us(50);

	RESET_EN(nibble);
	i2c_write(nibble);
	_delay_us(50);
}

void push_hight_nibble(uint8_t nibble, uint8_t light_is_on, uint8_t is_command)
{
	push_nibble((nibble & 0xF0), light_is_on, is_command);
}

void push_low_nibble(uint8_t nibble, uint8_t light_is_on, uint8_t is_command)
{
	push_nibble((nibble << 4), light_is_on, is_command);
}

void lcd_send_command(uint8_t command, uint8_t light_is_on)
{
	push_hight_nibble(command, light_is_on, COMMAND);
	push_low_nibble(command, light_is_on, COMMAND);
}

void lcd_send_data(uint8_t data, uint8_t light_is_on)
{
	push_hight_nibble(data, light_is_on, DATA);
	push_low_nibble(data, light_is_on, DATA);
}

uint8_t lcd_init(void)
{
	uint8_t value = 0;
	uint8_t error = 0;

	// ��� �� ��������� �������, ����� 4.5�
	_delay_ms(50);

	// ��������� i2c ���������
	error = i2c_start(PCF857A_WRITE);

	push_nibble(0x30, LIGHT_OFF, COMMAND);
	_delay_ms(50);
	push_nibble(0x30, LIGHT_OFF, COMMAND);
	_delay_ms(50);
	push_nibble(0x30, LIGHT_OFF, COMMAND);
	_delay_ms(50);

	// ������������� 4-� ������ �����
	SET_EN(value);
	SET_D5(value);
	SET_LIGHT(value);
	error |= (error << i2c_write(value));
	_delay_ms(50);
	RESET_EN(value);
	error |= (error << i2c_write(value));
	_delay_ms(50);

	// ��� ������, ������� 5�8
	lcd_send_command(0x28, LIGHT_ON);

	// �������� �������
	lcd_send_command(0x0C, LIGHT_ON);

	// ������� �������
	lcd_send_command(0x01, LIGHT_ON);

	// ��������� ��������, ������
	// ������������� ��������� ������
	// ��� ������
	lcd_send_command(0x06, LIGHT_ON);

	// ������������� i2c
	i2c_stop();

	return error;
}

void lcd_clear(uint8_t light_is_on)
{
	i2c_start(PCF857A_WRITE);
	lcd_send_command(CLEAR, light_is_on);
	i2c_stop();
	_delay_ms(100);
}

void lcd_light(uint8_t on_off)
{
	// TODO
}

void lcd_print(char* str, uint8_t line, uint8_t light_is_on)
{
	i2c_start(PCF857A_WRITE);
	lcd_send_command(0x0C, light_is_on);
	lcd_send_command(line, light_is_on);
	for (uint8_t i = 0; str[i]; i++)
	{
		lcd_send_data(str[i], light_is_on);
	}

	i2c_stop();
}

void lcd_print_line1(char* str, uint8_t light_is_on)
{
	lcd_print(str, LINE_1, light_is_on);
}

void lcd_print_line2(char* str, uint8_t light_is_on)
{
	lcd_print(str, LINE_2, light_is_on);
}