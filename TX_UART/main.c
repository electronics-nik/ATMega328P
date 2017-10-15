/*
 * TX_UART.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 *
 * Created: 15.10.2017 13:44:59
 * Author : Nikolay
 */ 

#include "tx_uart.h"

int main(void)
{
    tx_uart_init();
	tx_uart_puts("Test string\r\n");


	while (1) 
    {
		// tx_uart_putc('1');
    }
}

