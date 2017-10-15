/*
 * I2C.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 21:26:27
 * Author : Nikolay
 */ 

#include "i2c.h"
#include "tx_uart.h"

#define MAX_ADDRESS		255

int main(void)
{
    uint16_t address = 0;
    tx_uart_init();
    i2c_init();
    char buffer[ 255 ] = {};
    uint8_t answer;
    
    tx_uart_puts( "Start\r\n" );

    while ( address <= MAX_ADDRESS )
    {
	    answer = i2c_start( address );
	    sprintf( buffer, "address= %02X  answer= %s\r\n", address, (answer ? "No device" : "Connected to device") );
	    tx_uart_puts( buffer );
	    address++;

	    if ( !answer )
	    {
		    i2c_stop();
	    }

	    _delay_ms(500);
    }

    tx_uart_puts( "Stop\r\n" );
}

