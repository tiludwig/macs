/*
 * macs.c
 *
 * Created: 23.10.2018 15:20:12
 * Author : Tim
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "units/execution.h"
#include "serial/serial.h"
#include "torquer/torquer.h"
#include "parser.h"

int main(void)
{
	serial_init(UBRR);
	serial_puts("MACS FW V1.0 OCT 2018\r");
		
	yTorquerInitialize();
	xTorquerInitialize();
	zTorquerInitialize();
	
	torquerEnable();
	
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
				}
				index = 0;
			}
		}
		
		switch(measureState)
		{
			case IDLE:
			{
				if((currentTime - lastMeasurementTime) >= 781)
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
					torquerEnable();
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

