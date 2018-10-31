/*
 * torquers.h
 *
 * Created: 24.10.2018 14:30:37
 *  Author: Tim
 */ 


#ifndef TORQUERS_H_
#define TORQUERS_H_

uint8_t torquer_z_forward = 1;
uint8_t torquer_z_off = 1;

uint8_t torquer_y_forward = 1;
uint8_t torquer_y_off = 1;


uint8_t torquer_x_forward = 1;
uint8_t torquer_x_off = 1;


/**
 * TORQUER X
 * 
 */
void torquer_x_switch_direction(uint8_t forward)
{
  torquer_x_forward = forward;
  if(forward == 1)
  {
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  }
  else
  {
    TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
  }
}

void torquer_x_turn_off()
{
  TCCR2A = 0;
  TCCR2B = 0;
}

void torquer_x_turn_on()
{
  
  torquer_x_switch_direction(torquer_x_forward);
  TCNT2 = 0;
  TCCR2B = (1 << CS22) | (1 << CS20);
}

/**
 * TORQUER Z
 * 
 */
void torquer_z_switch_direction(uint8_t forward)
{
  torquer_z_forward = forward;
  if(forward == 1)
  {
    TCCR0A = (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  }
  else
  {
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
  }
}

void torquer_z_turn_off()
{
  TCCR0A = 0;
  TCCR0B = 0;
}

void torquer_z_turn_on()
{
  torquer_z_switch_direction(torquer_z_forward);
  TCNT0 = 0;
  TCCR0B = (1 << CS12) | (1 << CS10);
}

/**
 * TORQUER Y
 * 
 */

void torquer_y_switch_direction(uint8_t forward)
{
  torquer_y_forward = forward;
  if(forward == 1)
  {
    TCCR1A = (1 << COM1B1) | (1 << WGM10);
  }
  else
  {
    TCCR1A = (1 << COM1A1) | (1 << WGM10);
  }
}

void torquer_y_turn_off()
{
  TCCR1A = 0;
  TCCR1B = 0;
}

void torquer_y_turn_on()
{
  torquer_y_switch_direction(torquer_y_forward);
  TCNT1 = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}


#endif /* TORQUERS_H_ */