/*
 *	Command execution unit for MACS
 *
 *	Depending on the input value, the hardware timers have to be reconfigured to 
 *	output to a different channel or being completely stopped. 
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