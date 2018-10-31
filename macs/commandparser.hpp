/*
 * commandparser.c
 *
 * Created: 27.10.2018 00:19:34
 *  Author: Tim
 */ 

#include "commandparser.h"


static enum parserstate_t
{
	ST_IDLE,
	ST_WAITFORSIGN,
	ST_PARSE_X,
	ST_PARSE_Y,
	ST_PARSE_Z
};

static uint8_t parserState = ST_IDLE;

static inline int16_t str2int(const char *buffer, char terminator)
{
	int16_t result = 0;
	const char *buffer_ptr = buffer;
	while(*buffer_ptr != terminator)
	{
		if(*buffer_ptr >= '0' && *buffer_ptr <= '9')
		{
			result = 10 * result + (*buffer_ptr - '0');
		}
	}
	
	// check for sign
	if(*buffer == '-')
		result *= -1;
		
	return result;
}

struct setpoint_t decodeSetpointCommand(const char *buffer)
{
	uint8_t index = 0;
	struct setpoint_t setPoint;
	uint8_t x_negative = 0;
	uint8_t y_negative = 0;
	uint8_t z_negative = 0;
	do 
	{
		char currentData = *buffer;
		
		switch(parserState)
		{
			case ST_IDLE:
				if(currentData == 'D')
					parserState = ST_PARSE_X;
				break;
			case ST_PARSE_X:
				if(currentData != ' ')
				{
					setPoint.x = str2int(buffer, ' ');
					parserState = ST_PARSE_Y;
				}
				break;
			case ST_PARSE_Y:
				if(currentData != ' ')
				{
					setPoint.y = str2int(buffer, ' ');
					parserState = ST_PARSE_Y;
				}
				break;
			case ST_PARSE_Z:
				if(currentData != ' ')
				{
					setPoint.z = str2int(buffer, '\r');
					parserState = ST_IDLE;
				}
				break;
		}
		
	} while (*buffer++ != '\r');
	
	return setPoint;
}
