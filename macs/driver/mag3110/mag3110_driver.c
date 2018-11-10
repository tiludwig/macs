/*
 *	NXP MAG3110 Basic Driver
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

#include "mag3110_driver.h"
#include "mag3110_defines.h"
#include <avr/interrupt.h>
#include <util/twi.h>

/*
 *	Measurement ready interrupt
 *
 *	The MAG3110 will put the INT1 pin to HIGH, when a new measurement is available.
 *	This signals a new x, y OR z measurement, so the driver will have to wait until
 *	all three axis have completed measurement.
 *
 *	Gets enabled after a new measurement was triggered. 
 *	Disables itself after storing the measurement result.
 */
ISR(INT0_vect)
{
	
}

/*
 *	Initializes the MAG3110 to the following mode:
 *	
 *	Frequency:			0.63Hz
 *  Over-sampling rate: 128 (reduces output data rate to 0.08Hz)
 *  Reset:				automatic reset before every measurement
 *	Output format:		raw (no user offset correction)
 */
uint8_t mag3110_init()
{
	// Initialize TWI peripheral to i2c standard mode (100kbit/s)
	TWBR = 72;			// bit rate
	TWSR = 0;			// prescaler = 1
	TWCR = (1 << TWEN);	// enable twi peripheral
	
	return 0;
}

int8_t mag3110_isConnected()
{
	// query device id
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
	while  (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_START)
		return -1;
		
	// load register address 
	TWDR = MAG3110_WRITE_ADDRESS;
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while  (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
		return -2;
		
	TWDR = WHO_AM_I;
	TWCR = (1<<TWINT) | (1<<TWEN);
	
	while  (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
		return -3;
	
	// send repeated start and read
	TWCR = (1<<TWINT) | (1 << TWSTA) | (1<<TWEN);
	
	while  (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_REP_START)
		return -4;

	// load register address
	TWDR = MAG3110_READ_ADDRESS;	
	TWCR = (1<<TWINT) | (1<<TWEN);
		
	while  (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
		return -5;
	
	TWCR = (1<<TWINT) | (1<<TWEN);
		
	while  (!(TWCR & (1<<TWINT)));
	
	if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
		return -6;
	
	uint8_t device_id = TWDR;
	
	// send stop
	TWCR = (1<<TWINT) | (1 << TWSTO) | (1<<TWEN);
	
	while  (!(TWCR & (1<<TWINT)));
	
	if(device_id == MAG3110_DEVICE_ID)
		return 0;
	
	return -7;
}

/*
 *	Takes a measurement from the MAG3110
 *	
 *	Since the device is configured to active mode, triggering an immediate
 *	measurement will increase the output frequency to the highest level (10 Hz).
 *  A measurement result will be available after a maximum of 100ms.
 */
struct mag3110_result_t mag3110_takeMeasurement()
{
	struct mag3110_result_t measurements = {0, 0, 0};
	
	return measurements;
}
