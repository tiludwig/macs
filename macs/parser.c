/*
 * parser.c
 *
 * Created: 29.10.2018 18:14:57
 *  Author: Tim
 */ 

#include <string.h>
#include "parser.h"

static short str2short(const char *buffer)
{
	short tmpNum = 0;

	uint8_t isNegative = 0;
	if (*buffer == '-')
	{
		isNegative = 1;
		buffer++;
	}

	while (*buffer != '\0')
	{
		tmpNum = tmpNum * 10 + *buffer++ - '0';
	}

	return (isNegative == 1) ? -1*tmpNum : tmpNum;
}

uint8_t parseString(char* buffer, struct setpoint_t *setp)
{
	char* token = strtok(buffer, " ");
	if (strcmp(token, "D") != 0)
	{
		return PARSER_FAIL;
	}

	short dm[3];

	for (int i = 0; i < 3; i++)
	{
		// get next token
		token = strtok(0, " ");
		if (token == 0)
		{
			return PARSER_FAIL;
		}
		dm[i] = str2short(token);
	}
	
	setp->x = dm[0];
	setp->y = dm[1];
	setp->z = dm[2];
	return PARSER_OK;
}
