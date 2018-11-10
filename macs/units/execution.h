/*
 * execution.h
 *
 * Created: 08.11.2018 23:59:15
 *  Author: Tim
 */ 


#ifndef EXECUTION_H_
#define EXECUTION_H_

#include <avr/io.h>

#include <parser/parser.h>
#include <torquer/torquer.h>


extern uint8_t fastmap(uint8_t original);

static struct setpoint_t lastExecutedCmd;

void executeCommand(struct setpoint_t *cmd) {
	
	lastExecutedCmd.x = cmd->x;
	lastExecutedCmd.y = cmd->y;
	lastExecutedCmd.z = cmd->z;
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
#endif /* EXECUTION_H_ */