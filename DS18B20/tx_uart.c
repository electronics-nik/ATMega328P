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

 void tx_uart_put_int( const int value )
 {
	 char buffer[10];
	 tx_uart_puts( itoa( value, buffer, 10 ) );
 }

 void tx_uart_put_longint( const uint32_t value )
 {
	 char buffer[15];
	 tx_uart_puts( ltoa( value, buffer, 10 ) );
 }

 void tx_uart_put_ulongint( const uint32_t value )
 {
	 char buffer[15];
	 tx_uart_puts( ultoa( value, buffer, 10 ) );
 }

 void tx_uart_puthex_nibble(const uint8_t value)
 {
	 uint8_t c = value & 0x0f;
	 if ( c > 9 )
	 {
		 c += 'A'-10;
	 }
	 else
	 {
		 c += '0';
	 }

	 tx_uart_putc(c);
 }

 void tx_uart_puthex_byte( const uint8_t value )
 {
	 tx_uart_puthex_nibble( value >> 4 );
	 tx_uart_puthex_nibble( value );
 }

 void tx_uart_puthex_long( const uint32_t value )
 {
	 tx_uart_puthex_byte( (uint8_t)( value >> 24 ) );
	 tx_uart_puthex_byte( (uint8_t)( value >> 16 ) );
	 tx_uart_puthex_byte( (uint8_t)( value >> 8 ) );
	 tx_uart_puthex_byte( (uint8_t)( value ) );
 }

 void tx_uart_putbin_byte( const uint8_t value )
 {
	 for ( int8_t i= 7; i >= 0; i-- )
	 {
		 if ( value & ( 1 << i ) )
		 {
			 tx_uart_putc( '1' );
		 }
		 else
		 {
			 tx_uart_putc( '0' );
		 }
	 }
 }