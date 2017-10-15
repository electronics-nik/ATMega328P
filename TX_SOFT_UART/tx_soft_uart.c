/*
 * tx_soft_uart.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 23:27:54
 *  Author: Nikolay
 */ 

 // битрейт Ч 9600, при желании можно помен€ть. 
 // дл€ этого мен€ем задержку в функции UART_bit

 #include "tx_soft_uart.h"

 void tx_soft_uart_put_bit( uint8_t b )
 {
	if ( b == 0x00 )
	{
		TX_PORT &= ~( 1 << TX_PIN );
	}
	
	_delay_ms( 0.104 );
	TX_PORT |= ( 1 << TX_PIN );
 }

 void tx_soft_uart_putc( uint8_t a )
 {
	tx_soft_uart_put_bit( 0x00 );	//start bit

	for ( int i = 0; i < 8; i++ )
	{
		if ( ( a >> i ) & 0x01 )
		{
			tx_soft_uart_put_bit( 0x01 );	//data bit
		}
		else
		{
			tx_soft_uart_put_bit( 0x00 );	//data bit
		}
	}

	tx_soft_uart_put_bit( 0x01 );	//stop bit
 }

 void tx_soft_uart_init( void )
 {
	TX_DDR |= ( 1 << TX_PIN );
	TX_PORT |= ( 1 << TX_PIN );
 }

 void tx_soft_uart_puts( char *str )
 {
	uint8_t i = 0;
	while ( str[ i ] )
	{
		tx_soft_uart_putc( str[ i++ ] );
	}
 }