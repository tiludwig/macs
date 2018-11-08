/*
 * serial.c
 *
 * Created: 23.10.2018 15:21:32
 *  Author: Tim
 */ 
#include <avr/interrupt.h>
#include "serial.h"
#include "ringbuffer.h"

volatile struct ringbuffer_t rb_recv;
volatile struct ringbuffer_t rb_transmit;

ISR(USART_RX_vect)
{
	uint8_t data = UDR0;
	/* write data into buffer */
	rb_write(&rb_recv, data);
}



void serial_init(uint16_t ubrr)
{
	rb_init(&rb_recv);
	rb_init(&rb_transmit);
	
	cli();
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr;
	
	/* Enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	/* Set frame format 8 data, 1 stop*/
	UCSR0C = (3 << UCSZ00);
	sei();
}

void serial_putc(char character)
{
	/* Wait for empty transmit buffer */
	while( !(UCSR0A & (1 << UDRE0)));
	/* Transmit character */
	UDR0 = character;
}

void serial_puts(const char *string)
{
	while( *string != '\0')
	{
		serial_putc(*string);
		string++;
	}
}

uint8_t serial_available()
{
	return (rb_isEmpty(&rb_recv) == 0);	
}

uint8_t serial_getchar()
{
	return rb_read(&rb_recv);
}