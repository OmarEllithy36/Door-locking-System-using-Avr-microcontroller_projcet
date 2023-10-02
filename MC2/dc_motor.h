/******************************************************************************
 *
 * Module: DC_MOTOR
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the AVR DC_MOTOR driver
 *
 * Author: Omar Ellithy
 *
 *******************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include"data_types.h"
#include"gpio.h"

/*******************************************************************************
 *                              Definitions                        *
 *******************************************************************************/

/*
 * DC motor Configuration for Enable or Disable the motors
 * Write LOGIC_HIGH To Enable & LOGIC_LOW To Disable
 */
#define MOTOR1   LOGIC_HIGH
#define MOTOR2	 LOGIC_LOW

#if(MOTOR1 == LOGIC_HIGH)
/*
 * DC motor 1 select port that connect to input 1&2
 * Write the required port name "PORTA_ID,PORTB_ID,PORTC_ID,PORTD_ID"
 */
#define INP1_PORT    PORTC_ID
#define INP2_PORT    PORTC_ID

/*
 * DC motor 1 select pins that connect to input 1&2
 * Write the required pin name "PIN0_ID,PIN1_ID,PIN2_ID,PIN3_ID,PIN4_ID,PIN5_ID,PIN_ID,PIN7_ID"
 */
#define INP1_PIN     PIN3_ID
#define INP2_PIN     PIN4_ID

#endif

#if(MOTOR2 == LOGIC_HIGH)
/*
 * DC motor 2 select port that connect to input 3&4
 * Write the required port name "PORTA_ID,PORTB_ID,PORTC_ID,PORTD_ID"
 */
#define INP3_PORT  PORTC_ID
#define INP4_PORT  PORTC_ID

/*
 * DC motor 2 select pins that connect to input 3&4
 * Write the required pin name "PIN0_ID,PIN1_ID,PIN2_ID,PIN3_ID,PIN4_ID,PIN5_ID,PIN_ID,PIN7_ID"
 */
#define INP3_PIN  PIN2_ID
#define INP4_PIN  PIN3_ID

#endif


/*******************************************************************************
 *                            Types Declaration                    *
 *******************************************************************************/

typedef enum{
	STOP, CLOCK_WISE, ANTI_CLOCK_WISE
}motor_direction;

typedef enum{
	MOTOR_OFF,SPEED25=512,SPEED50=1024,SPEED75=1536,SPEED_MAX=2048
}motor_speed;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

#if(MOTOR1 == LOGIC_HIGH)

/*
 * Description :
 * The Function responsible for setup the direction for motor 1 pins through the GPIO driver
 * Stop the DC motor 1 at the beginning through the GPIO driver
 */
void dcMotor1_init(void);

/*
 * Description :
 * The function responsible for rotate the DC motor 1 CW/ or A-CW or stop the motor based on
 * the state input state value.
 * Control motor 1 speed by PWM driver
 */
void dcMotor1SetDirection(motor_direction direction1,motor_speed speed1);
void dcMotor1_deinit(void);
#endif

#if(MOTOR2 == LOGIC_HIGH)

/*
 * Description :
 * The Function responsible for setup the direction for motor 2 pins through the GPIO driver
 * Stop the DC motor 2 at the beginning through the GPIO driver
 */
void dcMotor2_init(void);

/*
 * Description :
 * The function responsible for rotate the DC motor 2 CW/ or A-CW or stop the motor based on
 * the state input state value.
 * Control motor 2 speed by PWM driver
 */
void dcMotor2SetDirection(motor_direction direction2,motor_speed speed2);

#endif

#endif /* DC_MOTOR_H_ */
