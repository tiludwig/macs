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
#include <driver/twi/twi.h>
#include <util/delay.h>

static volatile uint8_t measurementReady = 0;

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
	measurementReady = 1;
}

/*
 *	Initializes the MAG3110 to the following mode:
 *	
 *	Mode:				Standby
 *	Frequency:			80Hz
 *  Over-sampling rate: 128 (reduces output data rate to 10Hz)
 *  Reset:				Automatic reset before every measurement
 *	Output format:		With offset correction
 */
uint8_t mag3110_init()
{
	// Enable mag_ready pin to input, no pullup.
	MAG_INT_DDR &= ~(1 << MAG_INT_PIN);
	MAG_INT_PORT &= ~(1 << MAG_INT_PIN);
	
	EICRA = (3 << ISC00);
	EIMSK = (1 << INT0);
	sei();
	
	_delay_ms(2);
	twi_init();
	
	// check if magnetometer is connected
	if(mag3110_isConnected() == 0)
	{
		return 0;
	}
	
	// connected -> configure magnetometer
	uint8_t offsets[] = {0xF5, 0x46, 0xD, 0x12, 0x8, 0x1C};
	// apply user correction
	if(twi_writeByteArrayToAddress(MAG3110_ADDRESS, OFF_X_MSB, offsets, 6) == TWI_DRIVER_FAIL)
	{
		return 0;
	}
	
	
	uint8_t ctrl_reg[2];
	ctrl_reg[0] = (3 << CTRL1_OS0);
	ctrl_reg[1] = (1 << CTRL2_AUTO_MRST_EN);
	
	if(twi_writeByteArrayToAddress(MAG3110_ADDRESS, CTRL_REG1, ctrl_reg, 2) == TWI_DRIVER_FAIL)
	{
		return 0;
	}
		 
	
 	return 1;
}

int8_t mag3110_isConnected()
{
	if(twi_requestByteFrom(MAG3110_ADDRESS, WHO_AM_I) == MAG3110_DEVICE_ID)
	{
		return 1;
	}
	
	return 0;
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
	struct mag3110_result_t measurements = {0xFFFF, 0xFFFF, 0xFFFF};
		
	measurementReady = 0;
	// trigger measurement and enter standby
	uint8_t ctrl_reg1 = (3 << CTRL1_OS0) | (1 << CTRL1_TM);
	if(twi_writeByteToAddress(MAG3110_ADDRESS, CTRL_REG1, ctrl_reg1) == TWI_DRIVER_FAIL)
	{
		return measurements;
	}
	
	// wait for measurement ready
	while(measurementReady == 0);
	
	int numberOfAxisRead = 0;
	uint8_t status = 0;
	while(numberOfAxisRead != 3)
	{
		status = twi_requestByteFrom(MAG3110_ADDRESS, DR_STATUS);
		// try to read x
		if(status & (1 << STATUS_XDR))
		{
			uint8_t buffer[2];
			twi_RequestMultiByteFrom(MAG3110_ADDRESS, OUT_X_MSB, buffer, 2);
			measurements.x = ((uint16_t)buffer[0] << 8) | buffer[1];
			numberOfAxisRead++;
		}
		
		// try to read y
		if(status & (1 << STATUS_YDR))
		{
			uint8_t buffer[2];
			twi_RequestMultiByteFrom(MAG3110_ADDRESS, OUT_Y_MSB, buffer, 2);
			measurements.y = ((uint16_t)buffer[0] << 8) | buffer[1];
			numberOfAxisRead++;
		}
		
		// try to read z
		if(status & (1 << STATUS_YDR))
		{
			uint8_t buffer[2];
			twi_RequestMultiByteFrom(MAG3110_ADDRESS, OUT_Z_MSB, buffer, 2);
			measurements.z = ((uint16_t)buffer[0] << 8) | buffer[1];
			numberOfAxisRead++;
		}
	}
	
	return measurements;
}
