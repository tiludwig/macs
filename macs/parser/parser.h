/*
 * parser.h
 *
 * Created: 29.10.2018 18:14:34
 *  Author: Tim
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