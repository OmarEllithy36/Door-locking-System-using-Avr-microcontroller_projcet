/*
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the AVR Timers driver
 *
 * Author: Omar Ellithy
 */

#include "avr/io.h"
#include "timer.h"
#include "avr/interrupt.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */

static volatile void (*g_timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer2_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_COMP_vect)
{
	if(g_timer0_callBackPtr != NULL_PTR){
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer0_callBackPtr)();
	}
}

ISR(TIMER0_OVF_vect)
{
	if(g_timer0_callBackPtr != NULL_PTR){
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer0_callBackPtr)();
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_timer2_callBackPtr != NULL_PTR){
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer2_callBackPtr)();
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_timer2_callBackPtr != NULL_PTR){
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer2_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_timer1_callBackPtr != NULL_PTR){
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer1_callBackPtr)();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_timer1_callBackPtr != NULL_PTR){
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer1_callBackPtr)();
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void timer0_init(const timer_config *configure)
{
	/* set initial value to timer0  */
		TCNT0 = configure->initialValue;

	/* set compare value to timer0 */
		OCR0 = configure->compValue_REG;

	/* enable CTC & Normal Mode interrupt */
		switch(configure->mode){
			case OVERFLOW: TIMSK |= (1<<TOIE0); break;
			case COMPARE: TIMSK |= (1<<OCIE0); break;
		}


	/*
	 * TCCR0 register
	 * FOC0        = 1 for non PWM mode
	 * WGM0 1~0    = 0 for normal mode (overflow) and 2 for CTC mode
	 * COM0 1~0    = these bits control the output compare pin "OC0"
	 * 			   = 0 for disconnected & 1 for toggle & 2 clear on compare & 3 set on compare
	 * CS0 2~0     = clock prescaler select
	 */
	TCCR0 |= (1<<FOC0);
	switch(configure->mode){
		case OVERFLOW: TCCR0 &= ~(1<<WGM00); TCCR0 &= ~(1<<WGM01); break;
		case COMPARE: TCCR0 |= (1<<WGM01);  TCCR0 &= ~(1<<WGM00); break;
	}
	TCCR0 = (TCCR0 & 0xCF) | ((configure->OCMode) << 4);
	TCCR0 = (TCCR0 & 0xF8) | (configure->prescaler);
}

void timer0_setCallBack(void(*a_ptr) (void))
{
	/* Save the address of the Call back function in a global variable */
	g_timer0_callBackPtr = a_ptr;
}

void timer0_deinit(void)
{
	/* disable interrupt */
	TIMSK &= ~(1<<TOIE0);
	TIMSK &= ~(1<<OCIE0);

	/* reset TCCR0 register */
	TCCR0 = 0;
}


void timer2_init(const timer_config *configure)
{
	/* set initial value to timer0  */
		TCNT2 = configure->initialValue;

	/* set compare value to timer0 */
		OCR2 = configure->compValue_REG;

	/* enable CTC & Normal Mode interrupt */
		switch(configure->mode){
		    case OVERFLOW: TIMSK |= (1<<TOIE2); break;
			case COMPARE: TIMSK |= (1<<OCIE2); break;
		}

	/*
	 * TCCR0 register
	 * FOC0        = 1 for non pwm mode
	 * WGM2 1~0    = 0 for normal mode (overflow) and 2 for CTC mode
	 * COM2 1~0    = these bits control the output compare pin "OC2"
	 * 			   = 0 for disconnected & 1 for toggle & 2 clear on compare & 3 set on compare
	 * CS2 2~0     = clock prescaler select
	 */
	 TCCR2 |= (1<<FOC2);

	 switch(configure->mode){
		case OVERFLOW: TCCR2 &= ~(1<<WGM20); TCCR2 &= ~(1<<WGM21); break;
		case COMPARE: TCCR2 |= (1<<WGM21);  TCCR2 &= ~(1<<WGM20); break;
	 }
	 TCCR2 = (TCCR2 & 0xCF) | ((configure->OCMode) << 4);
	 TCCR2 = (TCCR2 & 0xF8) | (configure->prescaler);
}

void timer2_setCallBack(void(*a_ptr) (void))
{
	/* Save the address of the Call back function in a global variable */
	g_timer2_callBackPtr = a_ptr;
}

void timer2_deinit(void)
{
	/* disable interrupt */
	TIMSK &= ~(1<<TOIE2);
	TIMSK &= ~(1<<OCIE2);

	/* reset TCCR0 register */
	TCCR2 = 0;
}

void timer1_init(const timer_config *configure)
{
	TCNT1 = configure->initialValue;   /* Set timer1 initial value */

	OCR1A = configure->compValue_REG;  /* Set the Compare value to timer1 */

	/* Enable Timer1 Compare A Interrupt */
	switch(configure->mode){
		case OVERFLOW: TIMSK |= (1<<TOIE1); break;
		case COMPARE:  TIMSK |= (1<<OCIE1A); break;
	}

	/*
	 * TCCR1A register:
	 * COM1A 1~0     = these bits control the output compare pin "OC1A"
	 * 				 0 for disconnected & 1 for toggle & 2 clear on compare & 3 set on compare
	 * COM1B 1~0     = these bits control the output compare pin "OC1B" NOTE: channel B will implement later
	 * FOC1A & FOC1B = 1 for non PWM mode
	 * WGM1 1~0      = 0 for normal mode (overflow) and 0 for CTC mode
	 */

	/* set FOC1 A&B & clear WGM1 1&0 */
	TCCR1A = 0x0C;
	/* configure COM1A */
	TCCR1A = (TCCR1A & 0x3F) | ((configure->OCMode) << 6);

	/*
	 * TCCR1B register:
	 * ICNC1 & ICES1 = these bits responsible for ICU mode 0 to disable
	 * Bit 5 RESERVED
	 * WGM13         = 0 for CTC & NormalMode
	 * WGM12         = 0 NormalMode & 1 for compare mode
	 * CS1 2~0       = clock prescaler select
	 */

	/* disable ICU's bits and clear WGM13 */
	TCCR1B = 0x10;

	switch(configure->mode){
	case OVERFLOW: TCCR1B &= ~(1<<WGM12); break;
	case COMPARE:  TCCR1B |= (1<<WGM12);  break;
	}

	TCCR1B = (TCCR1B & 0xF8) | (configure->prescaler);
}

void timer1_setCallBack(void(*a_ptr) (void))
{
	/* Save the address of the Call back function in a global variable */
	g_timer1_callBackPtr = a_ptr;
}

void timer1_deinit(void)
{
	/* disable interrupt */
	TIMSK &= ~(1<<TOIE1);
	TIMSK &= ~(1<<OCIE1A);

	/* reset TCCR1 register */
	TCCR1A = 0;
	TCCR1B &= 0xC0;  /* clear all bits except ICU bits */
}
