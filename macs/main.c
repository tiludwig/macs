/*
 *	Magnetic Attitude Control System (MACS).
 *	
 *  This software controls the macs board, based on received dipole moment setpoints. 
 *  Utilizing a NXP MAG3110 it measures the surrounding magnetic field every 5 seconds
 *  and provides the measurement to the host PC.
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

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "units/execution.h"
#include "driver/uart/uart_driver.h"
#include "torquer/torquer.h"
#include "parser/parser.h"

/*
 *	Local 
 */
static struct setpoint_t lastExecutedCmd;


int main(void)
{
	serial_init(MAKE_UBRR);
	serial_puts("MACS FW V1.0 OCT 2018\r");
		
	yTorquerInitialize();
	xTorquerInitialize();
	zTorquerInitialize();
	
	torquerDisableSleep();
	
	uint8_t index = 0;
	char buffer[32];
	struct setpoint_t setp;
	
	uint16_t lastMeasurementTime = getTimestamp();
	uint16_t currentTime;
	enum MEASURESTATE {IDLE, STOPING_TORQUER, MEASURING, STARTING_TORQER};
	enum MEASURESTATE measureState = IDLE;
    while (1) 
    {	
		currentTime = getTimestamp();
		if(serial_available())
		{
			char data = serial_getchar();
			if(data != '\r' && data != '\n')
			{
				buffer[index++] = data;
			}
			else
			{
				buffer[index] = '\0';
				if(parseString(buffer, &setp) == PARSER_OK)
				{
					executeCommand(&setp);
					// store last command executed
					lastExecutedCmd.x = setp.x;
					lastExecutedCmd.y = setp.y;
					lastExecutedCmd.z = setp.z;
				}
				index = 0;
			}
		}
		
		switch(measureState)
		{
			case IDLE:
			{
				if((currentTime - lastMeasurementTime) >= 65000)
				{
					lastMeasurementTime = currentTime;
					//serial_puts("Stoping torquer.\r");
					torquerDisableAll();
					measureState = STOPING_TORQUER;
				}
				break;
			}
			case STOPING_TORQUER:
			{
				if((currentTime - lastMeasurementTime) >= 196)
				{
					//serial_puts("starting measurement.\r");
					measureState = MEASURING;
				}
				break;
			}
			case MEASURING:
			{

				if((currentTime - lastMeasurementTime) >= 2*196)
				{
					//serial_puts("starting torquers.\r");
					
					// replay last command executed
					torquerDisableSleep();
					executeCommand(&lastExecutedCmd);
					
					measureState = STARTING_TORQER;
				}
				break;
			}
			case STARTING_TORQER:
			{
				if((currentTime - lastMeasurementTime) >= 3*196)
				{
					//serial_puts("nominal operation.\r");
					measureState = IDLE;
				}
				break;
			}
		}
    }
}

