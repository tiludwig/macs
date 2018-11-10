/*
 * serial.h
 *
 * Created: 23.10.2018 15:21:24
 *  Author: Tim
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

#define UBRR (F_CPU / 16 / BAUD - 1)

#include <avr/io.h>

void serial_init(uint16_t ubrr);

void serial_putc(char character);

void serial_puts(const char *string);

uint8_t serial_available();

uint8_t serial_getchar();

#endif /* SERIAL_H_ */