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

struct command_t {
	int16_t moments[3];
};

/*
 *	Parse the command.
 *  Format: D [-](0-200) [-](0-200) [-](0-200)\n
 */
struct command_t parseCommand(char *cmd, uint8_t length) {
	struct command_t command;
	uint8_t cnt = 0;
	uint8_t last_occurance = 2;
	/* split string by whitespace */
	for(int i = 2; i < length; i++) {
		if(cmd[i] == ' ') {
			cmd[i] = '\0';
			command.moments[cnt++] = atoi(&cmd[last_occurance]);
			last_occurance = i+1;
		}
	}
	
	command.moments[cnt++] = atoi(&cmd[last_occurance]);
	return command;
}


void executeCommand(struct command_t *cmd) {
	
	/* scale inputs */
	int16_t xCycle = cmd->moments[0];
	int16_t yCycle = cmd->moments[1];
	int16_t zCycle = cmd->moments[2];
	
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


void printCommand(struct command_t *cmd)
{
	serial_puts("NEW TARGET: [");
	char buffer[7];
	itoa(cmd->moments[0], buffer, 10);
	serial_puts(buffer);
	serial_putc(' ');
	itoa(cmd->moments[1], buffer, 10);
	serial_puts(buffer);
	serial_putc(' ');
	itoa(cmd->moments[2], buffer, 10);
	serial_puts(buffer);
	serial_puts("]\n");
}

int main(void)
{
	serial_init(UBRR);
	serial_putc(0x11);
	serial_puts("MACS FW V1.0 OCT 2018\n\n");
		
	yTorquerInitialize();
	xTorquerInitialize();
	zTorquerInitialize();
	
	torquerEnable();
	
	uint8_t index = 0;
	char buffer[32];
    while (1) 
    {				
		if(serial_available())
		{
			// pause
			//serial_putc(0x13);
			//if(parseNextCharacter(serial_getchar()) == PARSER_COMPLETE)
			//{
				//// get command and execute it
			//}
			
			//if(data != '\r' || data != '\n')
			//{
				//buffer[index++] = data;
			//}
			//else
			//{
				////serial_puts("CMD\n");
				//buffer[index] = '\0';
				//struct command_t cmd = parseCommand(buffer, index);
				////printCommand(&cmd);
				////serial_puts("EXEC CMD\n");
				//
				//executeCommand(&cmd);				
				//index = 0;
			//}
		}
		//else
		//{
			//// continue
			//serial_putc(0x11);
		//}
    }
}

