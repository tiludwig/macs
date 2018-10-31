/*
 * commandparser.h
 *
 * Created: 27.10.2018 00:15:54
 *  Author: Tim
 */ 


#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include <avr/io.h>

struct setpoint_t
{
	int16_t x;
	int16_t y;
	int16_t z;
};


struct setpoint_t decodeSetpointCommand(const char *buffer);

#endif /* COMMANDPARSER_H_ */