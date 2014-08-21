/*
 * Temperature.c
 *
 * Created: 21.08.2014 12:15:41
 *  Author: Vinkiril
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
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

void ReadTemperatureSensor(uint16_t* a, uint16_t* b)
{
	ADCSRB|=1<<MUX5;          //this bit buffered till ADMUX written to!
	ADMUX =0xc9;              // Select internal 1.6 volt ref, temperature sensor ADC channel
	ADCSRA=0x85;              //Enable ADC, not free running, interrupt disabled, clock divider 32 (250 KHz@ 8 MHz)
	while ((ADCSRB&(1<<AVDDOK))==0);  //wait for AVDD ok
	while ((ADCSRB&(1<<REFOK))==0);   //wait for ref ok 
	ADCSRA|=1<<ADSC;          //Start throwaway conversion
	while (ADCSRA&(1<<ADSC)); //Wait till done
	ADCSRA|=1<<ADSC;          //Start another conversion
	while (ADCSRA&(1<<ADSC)); //Wait till done
	
	uint16_t h, m, s;
	h=ADC;                    //Read adc
	h=11*h-2728+(h>>2);       //Convert to celcius*10 (should be 11.3*h, approximate with 11.25*h)
	ADCSRA=0;                 //disable ADC
	ADMUX=0;                  //turn off internal vref      
	m = h/10;
	s = h-10*m;
	
	*a = m;
	*b = s;	
}

int main(void)
{	
	uart0_init();
	uint16_t deg1, deg2;
	while(1)
	{
		ReadTemperatureSensor(&deg1, &deg2);
		printf("Temperature = %i.%i(C)\n", deg1, deg2);
		_delay_ms(200);
	}
}
