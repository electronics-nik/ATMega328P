/*
 * SOFT_UART.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 16.10.2017 1:07:33
 * Author : Nikolay
 */ 

#include "soft_uart.h"
#include "tx_uart.h"

int main(void)
{
	sei();
		
	char c;
	static const char pstring[] PROGMEM = "adapted for Atmel AVR and this demo by Martin Thomas\r\n";
    
	tx_uart_init();
	soft_uart_init();
	soft_uart_turn_rx_on();

	tx_uart_puts("Test hard UART\n");

	// "implicit" PSTR
	soft_uart_puts_P( "\nSoftuart Demo-Application\n" );

	// explicit PSTR
	soft_uart_puts_p( PSTR("generic softuart driver code by Colin Gittins\n" ) );

	// pstring defined with PROGMEM
	soft_uart_puts_p( pstring );
	
	// string "from RAM" 
	soft_uart_puts( "--\n" );

	while (true) 
    {
		if ( soft_uart_kbhit() ) {
			c = soft_uart_getchar();
			tx_uart_putc( c );
		}
    }
}

