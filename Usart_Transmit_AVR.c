/*
 * UsartTransmitt.c
 *
 * Created: 18.08.2014 11:45:25
 *  Author: Vinkiril
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>


#if !defined(F_CPU)
#	error F_CPU not defined
#endif

#define BAUD 4800 //(FOSC/16/MYUBRR)
const unsigned int MYUBRR = (F_CPU/16/BAUD - 1);

int uart_putchar(char c, FILE *stream) 
{

	if (c == '\n') uart_putchar('\r', stream);
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;

	return 0;
}


void uart0_init(void) 
{
	unsigned char baudrateDiv = MYUBRR;

	UBRR0H = baudrateDiv >> 8;
	UBRR0L = baudrateDiv;
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = 0x2f;//(1<<USBS0)|(1<<UCSZ00);

	fdevopen(uart_putchar, NULL);
	
}

int main()
 { 
	 uart0_init();

	 while (1)
	 {
		printf("Hello, world!\n");
		_delay_ms(1000);
	 }
	 
	 return 0;
}
