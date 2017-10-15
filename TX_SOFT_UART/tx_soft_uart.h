/*
 * tx_soft_uart.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 23:27:40
 *  Author: Nikolay
 */ 


#ifndef TX_SOFT_UART_H_
#define TX_SOFT_UART_H_

#include "../constants.h"

#define TX_PORT		PORTB
#define TX_DDR		DDRB
#define TX_PIN		1

void tx_soft_uart_init( void );
void tx_soft_uart_putc( uint8_t c );
void tx_soft_uart_puts( char *s );

#endif /* TX_SOFT_UART_H_ */