/*
 *	Torquer control for MACS
 *
 *	Torquers will be controlled by PWM-signals. These signals are generated by 
 *	the hardware timers with a resolution of 8-bit for half the input range, 
 *	9-bit for full input range. This is because negative dipole moments will
 *	cause a channel switch, so [-200, 0] -> 8-bit on channel B and [0, 200] ->
 *	8-bit on channel A.
 *
 *	Timer 0 is also used for time keeping at a resolution of 256 us per tick. The
 *	current time can be queried by calling the 'getTimestamp' function.
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "torquer.h"

static volatile uint16_t timebase = 0;

/*
 *	Timebase interrupt
 *
 *	Gets called every time the timer 0 rolls over to 0. This is equivalent to every
 *	256us. The timebase can be used to measure time differences between two calls to 
 *	the 'getTimestamp' function.
 */
ISR(TIMER0_OVF_vect)
{
	timebase++;
}

/*
 *	Gets the current system time
 *
 *	Time is measurement using timer 0. Because this timer also runs the z torquer,
 *	the system time is measured in 256us ticks. Use this function to compute time
 *	differences between two timestamps. 
 */
uint16_t getTimestamp()
{
	cli();
	uint16_t currentTime = timebase;
	sei();
	return currentTime;
}

/*
 *	Initializes the x torquer
 *
 *	After initialization the torquer is off.
 */
void xTorquerInitialize()
{
	/* x torquer is mapped to 16-bit timer 1 */
	/* configure i/o lines */
	TORQUER_SLEEP_DDR |= TORQUER_SLEEP_PIN;
	
	DDRB |= TIM1_CHANNEL_A_PIN | TIM1_CHANNEL_B_PIN;
	TIM1_CHANNEL_PORT &= ~(TIM1_CHANNEL_A_PIN | TIM1_CHANNEL_B_PIN);
}

/*
 *	Initializes the y torquer
 *
 *	After initialization the torquer is off.
 */
void yTorquerInitialize()
{
	TORQUER_SLEEP_DDR |= TORQUER_SLEEP_PIN;

	/* y torquer is mapped to 8-bit timer 2 */
	/* configure i/o lines */
	DDRB |= TIM2_CHANNEL_A_PIN;
	DDRD |= TIM2_CHANNEL_B_PIN;
	TIM2_CHANNEL_A_PORT &= ~TIM2_CHANNEL_A_PIN;
	TIM2_CHANNEL_B_PORT &= ~TIM2_CHANNEL_B_PIN;
}

/*
 *	Initializes the z torquer
 *
 *	After initialization the torquer is off.
 */
void zTorquerInitialize()
{
	
	TORQUER_SLEEP_DDR |= TORQUER_SLEEP_PIN;

	/* z torquer is mapped to 8-bit timer 0 */
	/* configure i/o lines */
	DDRD |= TIM0_CHANNEL_A_PIN | TIM0_CHANNEL_B_PIN;
	TIM0_CHANNEL_PORT &= ~(TIM0_CHANNEL_A_PIN | TIM0_CHANNEL_B_PIN);
	
	TCNT0 = 0;
	TCCR0A = TIM0_TCCR0A_BASE_CFG;
	TIMSK0 = (1 << TOIE0);
	TCCR0B = TIM0_TCCR0B_ENABLE;
	sei();
}

/*
 *	Disables the x torquer
 */
void xTorquerDisable()
{
	TCCR1A = 0;
	TCCR1B = TIM1_TCCR1B_DISABLE;
	
	TIM1_CHANNEL_PORT &= ~(TIM1_CHANNEL_A_PIN | TIM1_CHANNEL_B_PIN);
}

/*
 *	Disables the y torquer
 */
void yTorquerDisable()
{
	TCCR2A = 0;
	TCCR2B = TIM2_TCCR2B_DISABLE;
	TIM2_CHANNEL_A_PORT &= ~TIM2_CHANNEL_A_PIN;
	TIM2_CHANNEL_B_PORT &= ~TIM2_CHANNEL_B_PIN;
}

/*
 *	Disables the z torquer
 */
void zTorquerDisable()
{
	// disconnect output ports
	TCCR0A = TIM0_TCCR0A_BASE_CFG;
	// output low
	TIM0_CHANNEL_PORT &= ~(TIM0_CHANNEL_A_PIN | TIM0_CHANNEL_B_PIN);
}

/*
 *	Disables all torquers and enters sleep mode
 */
void torquerDisableAll()
{
	xTorquerDisable();
	yTorquerDisable();
	zTorquerDisable();
	TORQUER_ENABLE_SLEEP();
}

/*
 *	Disables sleep mode for all three torquers
 */
void torquerDisableSleep()
{
	TORQUER_DISABLE_SLEEP();
}


/*
 *	Enable the output channel A for the x torqer
 */
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

/*
 *	Enable the output channel B for the x torqer
 */
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

/*
 *	Enable the output channel A for the y torqer
 */
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

/*
 *	Enable the output channel B for the y torqer
 */
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

/*
 *	Enable the output channel A for the z torqer
 */
void zTorquerEnableChannelA()
{
	//TCCR0B = TIM0_TCCR0B_DISABLE;
	/* switch to channel A*/
	TCCR0A = TIM0_TCCR0A_CHANNEL_A;

	/* put inactive channel to low level */
	TIM0_CHANNEL_PORT &= ~TIM0_CHANNEL_B_PIN;
	/* re-enable timer */
	//TCCR0B = TIM0_TCCR0B_ENABLE;
}

/*
 *	Enable the output channel B for the z torqer
 */
void zTorquerEnableChannelB()
{
	//TCCR0B = TIM0_TCCR0B_DISABLE;
	/* switch to channel A*/
	TCCR0A = TIM0_TCCR0A_CHANNEL_B;

	/* put inactive channel to low level */
	TIM0_CHANNEL_PORT &= ~TIM0_CHANNEL_A_PIN;
	
	/* re-enable timer */
	//TCCR0B = TIM0_TCCR0B_ENABLE;
}

/*
 *	Sets the duty cycle for the x torquer
 *
 *	This will set the duty cycle off both channels. Since only one channel can be active 
 *	by design, this doesn't pose a problem. However, if the user is to manipulate the 
 *	timer registers outside of this module, this has to be kept in mind.
 */
void xTorquerSetDutyCycle(uint8_t dutyCycle)
{
	if(dutyCycle == 0)
		TCCR1B = TIM1_TCCR1B_DISABLE;
	OCR1A = dutyCycle;
	OCR1B = dutyCycle;
}

/*
 *	Sets the duty cycle for the y torquer
 *
 *	This will set the duty cycle off both channels. Since only one channel can be active 
 *	by design, this doesn't pose a problem. However, if the user is to manipulate the 
 *	timer registers outside of this module, this has to be kept in mind.
 */
void yTorquerSetDutyCycle(uint8_t dutyCycle)
{
	if(dutyCycle == 0)
		TCCR2B = TIM2_TCCR2B_DISABLE;
	OCR2A = dutyCycle;
	OCR2B = dutyCycle;
}

/*
 *	Sets the duty cycle for the z torquer
 *
 *	This will set the duty cycle off both channels. Since only one channel can be active 
 *	by design, this doesn't pose a problem. However, if the user is to manipulate the 
 *	timer registers outside of this module, this has to be kept in mind.
 */
void zTorquerSetDutyCycle(uint8_t dutyCycle)
{
	if(dutyCycle == 0)
	{
		zTorquerDisable();
	}
	if(dutyCycle > 229)
		dutyCycle = 229;
	
	OCR0A = dutyCycle;
	OCR0B = dutyCycle;
}