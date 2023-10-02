 /******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.c
 *
 * Description: Source file for the AVR DC_MOTOR driver
 *
 * Author: Omar Ellithy
 *
 *******************************************************************************/

#include"dc_motor.h"
#include"bit_math.h"
#include "pwm.h"


#if(MOTOR1 == LOGIC_HIGH)
void dcMotor1_init(void)
{	/*
	 *	Set 2 pin as output to connect with 
	 *	input 1&2 in H-Bridge
	 */
	GPIO_setupPinDirection(INP1_PORT, INP1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(INP2_PORT, INP2_PIN, PIN_OUTPUT);
	
	/*
	 *	send logic low to input 1&2 to stop the motor
	 *	as initial status 
	 */
	GPIO_writePin(INP1_PORT, INP1_PIN, LOGIC_LOW);
	GPIO_writePin(INP2_PORT, INP2_PIN, LOGIC_LOW);

	PWM_Timer1_Init(0);
}

void dcMotor1SetDirection(motor_direction direction1,motor_speed speed1)
{
	if (direction1 == STOP) {
		GPIO_writePin(INP1_PORT, INP1_PIN, LOGIC_LOW);
		GPIO_writePin(INP2_PORT, INP2_PIN, LOGIC_LOW);
		
	}else if(direction1 == CLOCK_WISE) {
		GPIO_writePin(INP1_PORT, INP1_PIN, LOGIC_HIGH);
		GPIO_writePin(INP2_PORT, INP2_PIN, LOGIC_LOW);
		
	}else if(direction1 == ANTI_CLOCK_WISE){
		GPIO_writePin(INP1_PORT, INP1_PIN, LOGIC_LOW);
		GPIO_writePin(INP2_PORT, INP2_PIN, LOGIC_HIGH);
		
	}else{
		/* do nothing */
	}

	PWM_Timer1_Init(speed1);
}

void dcMotor1_deinit(void)
{
	PWM_Timer1_deinit();
}
#endif


#if(MOTOR2 == LOGIC_HIGH)
void dcMotor2_init(void)
{	/*
	 *	Set 2 pin as output to connect with 
	 *	input 3&4 in H-Bridge
	 */
	GPIO_setupPinDirection(INP3_PORT,INP3_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(INP4_PORT,INP4_PIN,PIN_OUTPUT);
	
	/*
	 *	send logic low to input 3&4 to stop the motor
	 *	as initial status 
	 */
	GPIO_writePin(INP3_PORT, INP3_PIN, LOGIC_LOW);
	GPIO_writePin(INP4_PORT, INP4_PIN, LOGIC_LOW);
}

void dcMotor2SetDirection(motor_direction direction2,motor_speed speed2)
{
	if(direction2 == STOP){
		GPIO_writePin(INP3_PORT, INP3_PIN, LOGIC_LOW);
		GPIO_writePin(INP4_PORT, INP4_PIN, LOGIC_LOW);
		
	}else if(direction2 == CLOCK_WISE){
		GPIO_writePin(INP3_PORT, INP3_PIN, LOGIC_HIGH);
		GPIO_writePin(INP4_PORT, INP4_PIN, LOGIC_LOW);
			
	}else if(direction2 == ANTI_CLOCK_WISE){
		GPIO_writePin(INP3_PORT, INP3_PIN, LOGIC_LOW);
		GPIO_writePin(INP4_PORT, INP4_PIN, LOGIC_HIGH);
		
	}else{
		/* do noting */
	}

	PWM_Timer0_Init(speed2);
}
#endif
