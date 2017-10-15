/*
 * TX_SOFT_UART.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 23:26:50
 * Author : Nikolay
 */ 

#include "tx_uart.h"
#include "tx_soft_uart.h"

int main(void)
{
	tx_uart_init();
	tx_soft_uart_init();

	tx_uart_putc( 'U' );
	tx_uart_putc( '\n' );

	tx_soft_uart_putc( 'U' );
	tx_soft_uart_putc( '\n' );

	tx_uart_puts( "Test string. Send to HARD UART\n" );
	tx_soft_uart_puts( "Test string. Send to SOFT UART\n" );

	char buffer[256] = {};
	uint8_t i = 0;

    while ( true ) 
    {
		sprintf( buffer, "tack - %d\n", i++ );
		tx_uart_puts( buffer );
		tx_soft_uart_puts( buffer );

		if( i == 30 )
		{
			break;
		}
    }
}

