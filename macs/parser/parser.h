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


#ifndef PARSER_H_
#define PARSER_H_

#define PARSER_OK		1
#define PARSER_FAIL		0

#include <avr/io.h>

struct setpoint_t
{
	int16_t x;
	int16_t y;
	int16_t z;	
};

uint8_t parseString(char* buffer, struct setpoint_t *setp);


#endif /* PARSER_H_ */