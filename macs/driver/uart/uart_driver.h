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


#ifndef SERIAL_H_
#define SERIAL_H_

#ifndef F_CPU
#error "Core clock frequency not defined (F_CPU)"
#endif

#ifndef BAUD
#warning "No baudrate defined. Defaulting to 9600"
#define BAUD 9600
#endif

#define MAKE_UBRR (F_CPU / 16 / BAUD - 1)

#include <avr/io.h>

/*
 *	Initializes the USART peripheral
 *	Use the MAKE_UBRR macro to get ubrr for a given baudrate.
 */
void serial_init(uint16_t ubrr);

/*
 *	Output  a character
 *	Outputs the character to the USART peripheral transmit buffer.
 *
 *	NOTE: This function blocks until the character is sent. At low 
 *	baudrates this time can be high.
 */
void serial_putc(char character);

/*
 *	Output  a c-string
 *	Outputs the c-string to the USART peripheral transmit buffer.
 *
 *	NOTE: This function blocks until the complete c-string is sent. 
 *	At low baudrates this time can be high.
 */
void serial_puts(const char *string);

/*
 *	Check if data is available
 *	Returns: 1, if data is available. 0 otherwise.
 */
uint8_t serial_available();

/*
 *	Get the next character from the receive buffer
 *	Returns: Data if available. 0 otherwise.
 *
 *	NOTE: 'serial_available' should be called prior to this function
 */
uint8_t serial_getchar();

#endif /* SERIAL_H_ */