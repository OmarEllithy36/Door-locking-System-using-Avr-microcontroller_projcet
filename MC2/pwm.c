/*
 * pwm.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#include "avr/io.h"
#include "pwm.h"

void PWM_Timer1_Init(uint16 a_dutyCycle)
{

	TCNT1 = 0;   /* Set Timer Initial value */

	OCR1A  = a_dutyCycle;   /* Set Compare Value */

	ICR1 = 2048;    /* set TOP value */

	DDRD  |= (1<<PD5);   /* set PD5/OC1A as output pin --> pin where the PWM signal is generated from MC. */

	/*
	 * TCCR1A register:
	 * COM1A1=1 & COMA10=0 : make "OC1" pin clear on compare match
	 * COM1B1=0 & COMB10=0 : channel B disconnect
	 * FOC1A=0 & FOC1B=1 : channel A PWM and B non PWM mode
	 * WGM11=1 & WGM11=0 : for fast PWM mode (mode 14)
	 */
	TCCR1A = 0x86;

	/*
	 * TCCR1B register:
	 * ICNC1 & ICES1 = these bits responsible for ICU mode 0 to disable
	 * Bit 5 RESERVED
	 * WGM13         = 1 for fast PWM (mode 14)
	 * WGM12         = 1 for fast PWM (mode 14)
	 * CS1 2~0       = 010 clock prescaler FCPU/8
	 */
	TCCR1B = 0x1A;
}

void PWM_Timer1_deinit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
}
