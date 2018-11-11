/*
 *	ATmega328p/PB TWI driver
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

#ifndef TWI_H_
#define TWI_H_

#define TWI_DRIVER_OK		1
#define TWI_DRIVER_FAIL		0

#include <avr/io.h>

/*
 *	Initialize the twi to 100kb/s
 */
uint8_t twi_init();

/*
 *	Send a start condition to the twi bus
 */
uint8_t twi_write_start();

/*
 *	Send a repeated start condition to the twi bus
 */
uint8_t twi_write_repeated_start();

/*
 *	Send the slave address in read mode
 */
uint8_t twi_write_sla_r(uint8_t address);

/*
 *	Send the slave address in write mode
 */
uint8_t twi_write_sla_w(uint8_t address);

/* 
 *	Send a byte to the twi bus
 */
uint8_t twi_write_byte(uint8_t data);

/*
 *	Read a single byte from the twi bus
 *
 *	After reading the byte signals NACK to the slave to stop reading
 */
uint8_t twi_read_single_byte();

/*
 *	Read a byte from the twi bus
 *
 *	After reading the byte signals ACK to the slave to continue reading
 */
uint8_t twi_read_multi_byte();


/*
 *	Send a stop conditiion to the twi bus
 */
uint8_t twi_write_stop();


/*
 *	Request the value of a byte address
 *
 *	slave: the slave address
 *	reg:   the byte address
 */
uint8_t twi_requestByteFrom(uint8_t slave, uint8_t reg);

/* 
 *	Request multiple bytes from the slave
 *
 *	slave: the slave address
 *	reg: the base address
 *	buffer: buffer to store data into
 *	count: how many bytes to read
 */
uint8_t twi_RequestMultiByteFrom(uint8_t slave, uint8_t reg, uint8_t *buffer, uint8_t count);


/*
 *	Write a byte to slave at address
 *
 *	slave: The slave address
 *	reg:   The byte address
 *	data:  The data to be written
 */
uint8_t twi_writeByteToAddress(uint8_t slave, uint8_t reg, uint8_t data);

/*
 *	Write a byte array to slave at address
 *
 *	slave: The slave address
 *	reg:   The base address
 *	data:  The data to be written
 *	count: The size of the array to be written
 */
uint8_t twi_writeByteArrayToAddress(uint8_t slave, uint8_t baseAddress, uint8_t *data, uint8_t count);

#endif /* TWI_H_ */