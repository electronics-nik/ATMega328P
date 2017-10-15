/*
 * tx_uart.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 13:46:54
 *  Author: Nikolay
 */ 

 #include "tx_uart.h" 

 #ifndef F_CPU
 #error "F_CPU not defined in <tx_uart.c>"
 #endif

 #ifndef BAUD_RATE
 #error "BAUD_RATE not defined in <tx_uart.c>"
 #endif

 void tx_uart_init( void )
 {
	 uint16_t value = ( ( F_CPU / 16 ) / ( BAUD_RATE - 1 ) );

	 UBRR0H = ( value >> 8 );
	 UBRR0L = value;
	 UCSR0B |= ( ( 1 << TXEN0 ) | ( 1 << RXEN0 ) | ( 1 << RXCIE0 ) );
	 UCSR0C |= ( ( 1 << USBS0 ) | ( 3 << UCSZ00 ) );
 }

 void tx_uart_putc( uint8_t c )
 {
	 while( !( UCSR0A & ( 1 << UDRE0 ) ) );
	 UDR0 = c;
 }

 void tx_uart_puts( char *s )
 {
	 while( *s )
	 {
		 tx_uart_putc( *s );
		 s++;
	 }
 }