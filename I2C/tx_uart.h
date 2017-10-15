/*
 * tx_uart.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 13:46:54
 *  Author: Nikolay
 */ 

#ifndef _TX_UART_H_
#define _TX_UART_H_ 
 
 #include "../constants.h"
  
 void tx_uart_init( void );
 void tx_uart_putc( uint8_t c );
 void tx_uart_puts( char *s );

#endif // _TX_UART_H_ 