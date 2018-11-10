/*
 *	ATmega328P/PB UART Driver
 *
 * Copyright 2018 Tim Ludwig
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */ 

#include <avr/interrupt.h>
#include "uart_driver.h"
#include "ringbuffer.h"

volatile struct ringbuffer_t rb_recv;
volatile struct ringbuffer_t rb_transmit;

/*
 *	USART data received interrupt
 *
 *	This interrupt will be called everytime a full byte is received by the USART
 *	peripheral. To reduce the risk of dropping data, because no other task is 
 *	reading the data, a ringbuffer is used to temporarely store the received data.
 */
ISR(USART_RX_vect)
{
	uint8_t data = UDR0;
	/* write data into buffer */
	rb_write(&rb_recv, data);
}


/*
 *	Initializes the USART peripheral
 *	Use the MAKE_UBRR macro to get ubrr for a given baudrate.
 */
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

/*
 *	Output  a character
 *	Outputs the character to the USART peripheral transmit buffer.
 *
 *	NOTE: This function blocks until the character is sent. At low 
 *	baudrates this time can be high.
 */
void serial_putc(char character)
{
	/* Wait for empty transmit buffer */
	while( !(UCSR0A & (1 << UDRE0)));
	/* Transmit character */
	UDR0 = character;
}

/*
 *	Output  a c-string
 *	Outputs the c-string to the USART peripheral transmit buffer.
 *
 *	NOTE: This function blocks until the complete c-string is sent. 
 *	At low baudrates this time can be high.
 */
void serial_puts(const char *string)
{
	while( *string != '\0')
	{
		serial_putc(*string);
		string++;
	}
}

/*
 *	Check if data is available
 *	Returns: 1, if data is available. 0 otherwise.
 */
uint8_t serial_available()
{
	return (rb_isEmpty(&rb_recv) == 0);	
}

/*
 *	Get the next character from the receive buffer
 *	Returns: Data if available. 0 otherwise.
 *
 *	NOTE: 'serial_available' should be called prior to this function
 */
uint8_t serial_getchar()
{
	return rb_read(&rb_recv);
}