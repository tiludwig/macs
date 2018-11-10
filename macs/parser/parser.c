/*
 *	MACS command parser
 *
 *  Provides functionality to parse a setpoint command. The command format is:
 *		ID [-]xxx [-]yyy [-]zzz\r
 *  Where ID is 'D' and xxx, yyy, zzz are the requested dipole moments along the 
 *	x, y and z axis respectively. Dipole moments are assumed to be positive until
 *  a '-' is prepended to the xxx, yyy or zzz values.
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

#include <string.h>
#include "parser.h"

/*
 *	Convert a string to short (int16_t)
 *	Returns: The numeric representation of the string value.
 */
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

/*
 *	Parse a command string
 *	Returns: PARSER_OK, if the command was parsed successfuly. PARSER_FAIL otherwise.
 *	
 *	buffer: The c-string to be parsed
 *	setp:	The setpoint struct containing the dipole moment target values. A allocated
 *			structure has to be supplied to the function. The function will alter the 
 *			x, y, and z components of the structure.
 */
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
