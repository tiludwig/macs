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

#include "twi.h"
#include <util/twi.h>

/*
 *	Initialize the twi to 400kb/s
 */
uint8_t twi_init()
{
	TWBR = 12;
	TWSR = 0;
	return TWI_DRIVER_OK;
}

/*
 *	Send a start condition to the twi bus
 */
uint8_t twi_write_start()
{
	// generate start condition
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_START)
		return TWI_DRIVER_FAIL;
		
	return TWI_DRIVER_OK;
}

/*
 *	Send a repeated start condition to the twi bus
 */
uint8_t twi_write_repeated_start()
{
	// generate start condition
	TWCR = (1 << TWSTA) | (1 << TWINT) | (1 << TWEN);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_REP_START)
	return TWI_DRIVER_FAIL;

	return TWI_DRIVER_OK;
}

/*
 *	Send the slave address in read mode
 */
uint8_t twi_write_sla_r(uint8_t address)
{
	TWDR = (address << 1) | 0x01;
	// transmit
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_MR_SLA_ACK)
		return TWI_DRIVER_FAIL;
	
	return TWI_DRIVER_OK;
}

/*
 *	Send the slave address in write mode
 */
uint8_t twi_write_sla_w(uint8_t address)
{
	TWDR = address << 1;
	// transmit
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_MT_SLA_ACK)
		return TWI_DRIVER_FAIL;
	
	return TWI_DRIVER_OK;
}


/* 
 *	Send a byte to the twi bus
 */
uint8_t twi_write_byte(uint8_t data)
{
	TWDR = data;
	// transmit
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_MT_DATA_ACK)
		return TWI_DRIVER_FAIL;
	
	return TWI_DRIVER_OK;
}

/*
 *	Read a single byte from the twi bus
 */
uint8_t twi_read_single_byte()
{
	// read byte and send NACK
	TWCR = (1 << TWINT) | (1 << TWEN);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_MR_DATA_NACK)
		return TWI_DRIVER_FAIL;
	
	return TWDR;
}

/*
 *	Reads multiple byte from the twi bus
 */
uint8_t twi_read_multi_byte()
{
	// read byte and send ACK
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	// wait for transaction to finish
	while( !(TWCR & (1 << TWINT)) );
	// check status
	if( (TWSR & 0xF8) != TW_MR_DATA_ACK)
		return TWI_DRIVER_FAIL;

	return TWDR;
}
/*
 *	Send a stop condition to the twi bus
 */
uint8_t twi_write_stop()
{
	// generate stop condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
	while(TWCR & (1<<TWSTO));
	
	return TWI_DRIVER_OK;
}

/*
 *	Request the value of a byte address
 *
 *	slave: the slave address
 *	reg:   the byte address
 */
uint8_t twi_requestByteFrom(uint8_t slave, uint8_t reg)
{
	if(twi_write_start() != TWI_DRIVER_OK)
	{
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_sla_w(slave) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_byte(reg) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_repeated_start() == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_sla_r(slave) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	uint8_t data = twi_read_single_byte();
	twi_write_stop();
	
	return data;
}


/* 
 *	Request multiple bytes from the slave
 *
 *	slave: the slave address
 *	reg: the base address
 *	buffer: buffer to store data into
 *	count: how many bytes to read
 */
uint8_t twi_RequestMultiByteFrom(uint8_t slave, uint8_t reg, uint8_t *buffer, uint8_t count)
{
	if(twi_write_start() != TWI_DRIVER_OK)
	{
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_sla_w(slave) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_byte(reg) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_repeated_start() == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_sla_r(slave) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}

	for(uint8_t i = 0; i < count -1; i++)
	{
		buffer[i] = twi_read_multi_byte();
	}

	buffer[count-1] = twi_read_single_byte();
	twi_write_stop();
	return TWI_DRIVER_OK;
}

/*
 *	Write a byte to slave at address
 *
 *	slave: The slave address
 *	reg:   The byte address
 *	data:  The data to be written
 */
uint8_t twi_writeByteToAddress(uint8_t slave, uint8_t reg, uint8_t data)
{
	if(twi_write_start() == TWI_DRIVER_FAIL)
	{
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_sla_w(slave) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_byte(reg) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_byte(data) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	twi_write_stop();
	return TWI_DRIVER_OK;
}

/*
 *	Write a byte array to slave at address
 *
 *	slave: The slave address
 *	reg:   The base address
 *	data:  The data to be written
 *	count: The size of the array to be written
 */
uint8_t twi_writeByteArrayToAddress(uint8_t slave, uint8_t baseAddress, uint8_t *data, uint8_t count)
{
	if(twi_write_start() == TWI_DRIVER_FAIL)
	{
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_sla_w(slave) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	if(twi_write_byte(baseAddress) == TWI_DRIVER_FAIL)
	{
		twi_write_stop();
		return TWI_DRIVER_FAIL;
	}
	
	for(uint8_t i = 0; i < count; i++)
	{
		if(twi_write_byte(data[i]) == TWI_DRIVER_FAIL)
		{
			twi_write_stop();
			return TWI_DRIVER_FAIL;
		}
	}
	
	
	twi_write_stop();
	return TWI_DRIVER_OK;
}