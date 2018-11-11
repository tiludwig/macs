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


#ifndef MAG3110_DRIVER_H_
#define MAG3110_DRIVER_H_

#define GLOBAL_FLAGS	_SFR_IO8(0x1E)
#define MAG_INT_DDR		DDRD
#define MAG_INT_PORT	PORTD
#define MAG_INT_PIN		PD2


#include <avr/io.h>

/*
 *	MAG3110 measurement result structure
 *	
 *	Entries correspond to the measured magnetic intensity on x,y and z axis.
 */
struct mag3110_result_t
{
	int16_t x;
	int16_t y;
	int16_t z;	
};

/*
 *	Initializes the MAG3110 to the following mode:
 *	
 *	Frequency:			0.63Hz
 *  Over-sampling rate: 128 (reduces output data rate to 0.08Hz)
 *  Reset:				automatic reset before every measurement
 *	Output format:		raw (no user offset correction)
 */
uint8_t mag3110_init();

int8_t mag3110_isConnected();

/*
 *	Takes a measurement from the MAG3110
 *	
 *	Since the device is configured to active mode, triggering an immediate
 *	measurement will increase the output frequency to the highest level (10 Hz).
 *  A measurement result will be available after a maximum of 100ms.
 */
struct mag3110_result_t mag3110_takeMeasurement();
#endif /* MAG3110_DRIVER_H_ */