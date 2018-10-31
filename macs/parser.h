/*
 * parser.h
 *
 * Created: 29.10.2018 18:14:34
 *  Author: Tim
 */ 


#ifndef PARSER_H_
#define PARSER_H_

#include <avr/io.h>

#define PARSER_WAITING		0
#define PARSER_COMPLETE		1
#define PARSER_ERROR		2



void minusReceived(uint8_t data)
{
	
}

void numberReceived(uint8_t data)
{
	
}

void errorDetected(uint8_t data)
{
	
}

void numberFinished(uint8_t data)
{
	
}

void commandFinished(uint8_t data)
{
	
}





enum EVENTS
{
	EVT_ID_RECVD,
	EVT_NUMBER_RECVD,
	EVT_MINUS_RECVD,
	EVT_WHITESPACE_RECVD,
	EVT_CARRIAGE_RECVD,
	EVT_NEWLINE_RECVD,
	EVT_INVALID_RECVD
};

struct transition_t
{
	uint8_t currentState;
	enum EVENTS inputEvent;
	uint8_t nextState;
	void (*entry_function)(uint8_t);	
};

struct transition_t statemachine[] =
{
	{0, EVT_ID_RECVD, 1, 0},
	{0, EVT_INVALID_RECVD, 0, 0},
	
	{1, EVT_WHITESPACE_RECVD, 1, 0},	
	{1, EVT_MINUS_RECVD, 3, minusReceived},	
	{1, EVT_NUMBER_RECVD, 2, numberReceived},	
	{1, EVT_INVALID_RECVD, 4, errorDetected},
		
	{2, EVT_WHITESPACE_RECVD, 2, numberReceived},
	{2, EVT_MINUS_RECVD, 1, numberFinished},
	{2, EVT_NUMBER_RECVD, 0, commandFinished},
	{2, EVT_INVALID_RECVD, 4, errorDetected},
			
	{3, EVT_WHITESPACE_RECVD, 2, numberReceived},
	{3, EVT_MINUS_RECVD, 4, minusReceived}	
};

#endif /* PARSER_H_ */