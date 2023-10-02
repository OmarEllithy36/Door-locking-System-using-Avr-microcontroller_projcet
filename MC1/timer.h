/*
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR Timers driver
 *
 * Author: Omar Ellithy
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "data_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	OVERFLOW,COMPARE
}timerMode;

typedef enum {
	DISCONNECTED,TOGGLE_PIN,CLEAR_PIN,SET_PIN
}OC_control;

typedef enum {
	NOPRESCALING=1,CLK_8,CLK_64,CLK256,CLK_1024,EXT_FALLING,EXT_RISING
}clockSelect;

typedef struct{
	timerMode mode;
	OC_control OCMode;
	clockSelect prescaler;
	uint8 initialValue;
	uint8 compValue_REG;
}timer_config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the timer0
 * 	1. Set the initial value to TCNT0 register
 * 	2. Set the required compare value to OCR0 register.
 * 	3. Set the required mode (CTC or overflow)
 * 	4. Enable CTC or Normal Mode Interrupt.
 * 	5. Control the output compare pin "OC0"
 * 	6. Select the required clock prescaler
 */
void timer0_init(const timer_config *configure);

/*
 * Description: Function to set the Call Back function address.
 */
void timer0_setCallBack(void(*a_ptr) (void));

/*
 * Description: Function to disable the Timer0
 */
void timer0_deinit(void);

/*
 * Description : Function to initialize the timer2
 * 	1. Set the initial value to TCNT2 register
 * 	2. Set the required compare value to OCR2 register.
 * 	3. Set the required mode (CTC or overflow)
 * 	4. Enable CTC or Normal Mode Interrupt.
 * 	5. Control the output compare pin "OC2"
 * 	6. Select the required clock prescaler
 */
void timer2_init(const timer_config *configure);

/*
 * Description: Function to set the Call Back function address.
 */
void timer2_setCallBack(void(*a_ptr) (void));

/*
 * Description: Function to disable the Timer2
 */
void timer2_deinit(void);

/*
 * Description : Function to initialize the timer1 channel A Only
 * 	1. Set the initial value to TCNT1A register
 * 	2. Set the required compare value to OCR1A register.
 * 	3. Set the required mode (CTC or overflow)
 * 	4. Enable CTC or Normal Mode Interrupt.
 * 	5. Control the output compare pin "OC1A"
 * 	6. Select the required clock prescaler
 */
void timer1_init(const timer_config *configure);

/*
 * Description: Function to set the Call Back function address.
 */
void timer1_setCallBack(void(*a_ptr) (void));

/*
 * Description: Function to disable the Timer1
 */
void timer1_deinit(void);

#endif /* TIMER_H_ */
