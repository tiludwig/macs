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

#include "mag3110_driver.h"
#include <avr/interrupt.h>

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

}
