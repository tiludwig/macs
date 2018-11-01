/*
 * torquer.h
 *
 * Created: 24.10.2018 14:32:53
 *  Author: Tim
 */ 


#ifndef TORQUER_H_
#define TORQUER_H_

#include <avr/io.h>

#define TORQUER_SLEEP_DDR			(DDRC)
#define TORQUER_SLEEP_PORT			(PORTC)
#define TORQUER_SLEEP_PIN			(1 << PC0)
#define TORQUER_ENABLE_SLEEP()		(TORQUER_SLEEP_PORT &= ~TORQUER_SLEEP_PIN)
#define TORQUER_DISABLE_SLEEP()		(TORQUER_SLEEP_PORT |= TORQUER_SLEEP_PIN)

#define TIM0_CHANNEL_PORT			(PORTD)
#define TIM0_CHANNEL_A_PIN			(1 << PD6)
#define TIM0_CHANNEL_B_PIN			(1 << PD5)
#define TIM0_TCCR0A_BASE_CFG		((1 << WGM01) | (1 << WGM00))
#define TIM0_TCCR0A_CHANNEL_A		((1 << COM0A1) | TIM0_TCCR0A_BASE_CFG)
#define TIM0_TCCR0A_CHANNEL_B		((1 << COM0B1) | TIM0_TCCR0A_BASE_CFG)
#define TIM0_TCCR0B_DISABLE			(0)
#define TIM0_TCCR0B_ENABLE			((1 << CS01))

#define TIM1_CHANNEL_A_PIN			(1 << PB1)
#define TIM1_CHANNEL_B_PIN			(1 << PB2)
#define TIM1_CHANNEL_PORT			(PORTB)
#define TIM1_TCCR1A_BASE_CFG		(1 << WGM10)
#define TIM1_TCCR1B_BASE_CFG		(1 << WGM12)
#define TIM1_TCCR1A_CHANNEL_A		((1 << COM1A1) | TIM1_TCCR1A_BASE_CFG)
#define TIM1_TCCR1A_CHANNEL_B		((1 << COM1B1) | TIM1_TCCR1A_BASE_CFG)
#define TIM1_TCCR1B_DISABLE			(0)
#define TIM1_TCCR1B_ENABLE			(TIM1_TCCR1B_BASE_CFG | (1 << CS11))

#define TIM2_CHANNEL_A_PORT			(PORTB)
#define TIM2_CHANNEL_B_PORT			(PORTD)
#define TIM2_CHANNEL_A_PIN			(1 << PB3)
#define TIM2_CHANNEL_B_PIN			(1 << PD3)
#define TIM2_TCCR2A_BASE_CFG		((1 << WGM21) | (1 << WGM20))
#define TIM2_TCCR2A_CHANNEL_A		((1 << COM2A1) | TIM2_TCCR2A_BASE_CFG)
#define TIM2_TCCR2A_CHANNEL_B		((1 << COM2B1) | TIM2_TCCR2A_BASE_CFG)
#define TIM2_TCCR2B_DISABLE			(0)
#define TIM2_TCCR2B_ENABLE			((1 << CS21))

struct TIM1
{
	uint8_t TCCRA;
	uint8_t TCCRB;
	uint8_t TCCRC;
	uint8_t TCNTL;
	uint8_t TCNTH;
	uint8_t ICRL;
	uint8_t ICRH;
	uint8_t OCRAL;
	uint8_t OCRAH;
	uint8_t OCRBL;
	uint8_t OCRBH;
};

struct TIM0
{
	uint8_t TCCRA;
	uint8_t TCCRB;
};

#define TIM1_BASE				(0x84)
#define TORQUER_X		((volatile struct TIM1 *)(TIM1_BASE))

#define TIM0_BASE				(0x44)
#define TORQUER_Z		((volatile struct TIM0*)(TIM0_BASE))

void xTorquerInitialize();
void yTorquerInitialize();
void zTorquerInitialize();

void xTorquerDisable();
void yTorquerDisable();
void zTorquerDisable();

void torquerEnable();
void torquerDisableAll();

void xTorquerEnableChannelA();
void xTorquerEnableChannelB();

void yTorquerEnableChannelA();
void yTorquerEnableChannelB();

void zTorquerEnableChannelA();
void zTorquerEnableChannelB();

void xTorquerSetDutyCycle(uint8_t dutyCycle);
void yTorquerSetDutyCycle(uint8_t dutyCycle);
void zTorquerSetDutyCycle(uint8_t dutyCycle);


#endif /* TORQUER_H_ */