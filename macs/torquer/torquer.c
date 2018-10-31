/*
 * torquer.c
 *
 * Created: 24.10.2018 14:34:43
 *  Author: Tim
 */ 

#include <avr/io.h>
#include "torquer.h"


void xTorquerInitialize()
{
	/* x torquer is mapped to 16-bit timer 1 */
	/* configure i/o lines */
	TORQUER_SLEEP_DDR |= TORQUER_SLEEP_PIN;
	TORQUER_ENABLE_SLEEP();
	DDRB |= TIM1_CHANNEL_A_PIN | TIM1_CHANNEL_B_PIN;
	TIM1_CHANNEL_PORT &= ~(TIM1_CHANNEL_A_PIN | TIM1_CHANNEL_B_PIN);
}

void yTorquerInitialize()
{
	TORQUER_SLEEP_DDR |= TORQUER_SLEEP_PIN;
	TORQUER_ENABLE_SLEEP();
	/* y torquer is mapped to 8-bit timer 2 */
	/* configure i/o lines */
	DDRB |= TIM2_CHANNEL_A_PIN;
	DDRD |= TIM2_CHANNEL_B_PIN;
	TIM2_CHANNEL_A_PORT &= ~TIM2_CHANNEL_A_PIN;
	TIM2_CHANNEL_B_PORT &= ~TIM2_CHANNEL_B_PIN;
}

void zTorquerInitialize()
{
	TORQUER_SLEEP_DDR |= TORQUER_SLEEP_PIN;
	TORQUER_ENABLE_SLEEP();
	/* z torquer is mapped to 8-bit timer 0 */
	/* configure i/o lines */
	DDRD |= TIM0_CHANNEL_A_PIN | TIM0_CHANNEL_B_PIN;
	TIM0_CHANNEL_PORT &= ~(TIM0_CHANNEL_A_PIN | TIM0_CHANNEL_B_PIN);
}


void xTorquerDisable()
{
	TCCR1A = 0;
	TCCR1B = TIM1_TCCR1B_DISABLE;
	
	TIM1_CHANNEL_PORT &= ~(TIM1_CHANNEL_A_PIN | TIM1_CHANNEL_B_PIN);
}

void yTorquerDisable()
{
	TCCR2A = 0;
	TCCR2B = TIM2_TCCR2B_DISABLE;
	TIM2_CHANNEL_A_PORT &= ~TIM2_CHANNEL_A_PIN;
	TIM2_CHANNEL_B_PORT &= ~TIM2_CHANNEL_B_PIN;
}

void zTorquerDisable()
{
	TCCR0A = 0;
	TCCR0B = TIM0_TCCR0B_DISABLE;
	// output low
	TIM0_CHANNEL_PORT &= ~(TIM0_CHANNEL_A_PIN | TIM0_CHANNEL_B_PIN);
}

void torquerDisableAll()
{
	xTorquerDisable();
	yTorquerDisable();
	zTorquerDisable();
	TORQUER_ENABLE_SLEEP();
}

void torquerEnable()
{
	TORQUER_DISABLE_SLEEP();
}

void xTorquerEnableChannelA()
{
	TCCR1B = TIM1_TCCR1B_DISABLE;
	/* switch to channel A*/
	TCCR1A = TIM1_TCCR1A_CHANNEL_A;
	
	/* put inactive channel to low level */
	TIM1_CHANNEL_PORT &= ~TIM1_CHANNEL_B_PIN;
	
	/* re-enable timer */
	TCCR1B = TIM1_TCCR1B_ENABLE;
}

void xTorquerEnableChannelB()
{
	TCCR1B = TIM1_TCCR1B_DISABLE;
	/* switch to channel A*/
	TCCR1A = TIM1_TCCR1A_CHANNEL_B;

	/* put inactive channel to low level */
	TIM1_CHANNEL_PORT &= ~TIM1_CHANNEL_A_PIN;

	/* re-enable timer */
	TCCR1B = TIM1_TCCR1B_ENABLE;
}

void yTorquerEnableChannelA()
{
	TCCR2B = TIM2_TCCR2B_DISABLE;
	/* switch to channel A*/
	TCCR2A = TIM2_TCCR2A_CHANNEL_A;
	
	/* put inactive channel to low level */
	TIM2_CHANNEL_B_PORT &= ~TIM2_CHANNEL_B_PIN;
	
	/* re-enable timer */
	TCCR2B = TIM2_TCCR2B_ENABLE;
}

void yTorquerEnableChannelB()
{
	TCCR2B = TIM2_TCCR2B_DISABLE;
	/* switch to channel A*/
	TCCR2A = TIM2_TCCR2A_CHANNEL_B;

	/* put inactive channel to low level */
	TIM2_CHANNEL_A_PORT &= ~TIM2_CHANNEL_A_PIN;
	/* re-enable timer */
	TCCR2B = TIM2_TCCR2B_ENABLE;
}

void zTorquerEnableChannelA()
{
	TCCR0B = TIM0_TCCR0B_DISABLE;
	/* switch to channel A*/
	TCCR0A = TIM0_TCCR0A_CHANNEL_A;

	/* put inactive channel to low level */
	TIM0_CHANNEL_PORT &= ~TIM0_CHANNEL_B_PIN;

	/* re-enable timer */
	TCCR0B = TIM0_TCCR0B_ENABLE;
}

void zTorquerEnableChannelB()
{
	TCCR0B = TIM0_TCCR0B_DISABLE;
	/* switch to channel A*/
	TCCR0A = TIM0_TCCR0A_CHANNEL_B;

	/* put inactive channel to low level */
	TIM0_CHANNEL_PORT &= ~TIM0_CHANNEL_A_PIN;

	/* re-enable timer */
	TCCR0B = TIM0_TCCR0B_ENABLE;
}

void xTorquerSetDutyCycle(uint8_t dutyCycle)
{
	if(dutyCycle == 0)
		TCCR1B = TIM1_TCCR1B_DISABLE;
	OCR1A = dutyCycle;
	OCR1B = dutyCycle;
}

void yTorquerSetDutyCycle(uint8_t dutyCycle)
{
	if(dutyCycle == 0)
		TCCR2B = TIM2_TCCR2B_DISABLE;
	OCR2A = dutyCycle;
	OCR2B = dutyCycle;
}

void zTorquerSetDutyCycle(uint8_t dutyCycle)
{
	if(dutyCycle == 0)
	{
		TCCR0B = TIM0_TCCR0B_DISABLE;
	}
	OCR0A = dutyCycle;
	OCR0B = dutyCycle;
}