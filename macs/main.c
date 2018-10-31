/*
 * macs.c
 *
 * Created: 23.10.2018 15:20:12
 * Author : Tim
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "serial/serial.h"
#include "torquer/torquer.h"
#include "parser.h"

extern uint8_t fastmap(uint8_t original);

void executeCommand(struct setpoint_t *cmd) {
	
	/* scale inputs */
	int16_t xCycle = cmd->x;
	int16_t yCycle = cmd->y;
	int16_t zCycle = cmd->z;
	
	if(xCycle < 0)
	{
		xCycle *= -1;
		xTorquerSetDutyCycle(fastmap(xCycle));
		xTorquerEnableChannelA();
	}
	else if(xCycle == 0)
	{
		xTorquerDisable();
	}
	else
	{
		xTorquerSetDutyCycle(fastmap(xCycle));
		xTorquerEnableChannelB();
	}
	
	if(yCycle < 0)
	{
		yCycle *= -1;
		yTorquerSetDutyCycle(fastmap(yCycle));
		yTorquerEnableChannelA();
	}
	else if(yCycle == 0)
	{
		yTorquerDisable();
	}
	else
	{
		yTorquerSetDutyCycle(fastmap(yCycle));
		yTorquerEnableChannelB();
	}

	if(zCycle < 0)
	{
		zCycle *= -1;
		zTorquerSetDutyCycle(fastmap(zCycle));
		zTorquerEnableChannelA();
	} 
	else if(zCycle == 0)
	{
		zTorquerDisable();
	}
	else
	{
		zTorquerSetDutyCycle(fastmap(zCycle));
		zTorquerEnableChannelB();
	}
}


void printCommand(struct setpoint_t *cmd)
{
	serial_puts("NEW TARGET: [");
	char buffer[7];
	itoa(cmd->x, buffer, 10);
	serial_puts(buffer);
	serial_putc(' ');
	itoa(cmd->y, buffer, 10);
	serial_puts(buffer);
	serial_putc(' ');
	itoa(cmd->z, buffer, 10);
	serial_puts(buffer);
	serial_puts("]\r");
}

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
    while (1) 
    {				
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
    }
}

