/*
 * function2.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#include "data_types.h"
#include "uart.h"
#include "i2c.h"
#include "external_eeprom.h"
#include "util/delay.h"
#include "application2.h"
#include "buzzer.h"
#include "timer.h"
#include "dc_motor.h"


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static const uint16 firstPass_location = 0x0010;
static uint16 g_tick = 0;
static uint8 g_flag = FALSE;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

static uint8 comparePass(const uint8 password_arr[][5])
{
	uint8 isEqual = FALSE;
	uint8 index;

	for(index=0; index<NUMOf_PASS_DIGITS; index++){
		if(password_arr[0][index] == password_arr[1][index]){
			isEqual = TRUE;
		}else{
			isEqual = FALSE;
			break;
		}
	}

	return isEqual;
}

static uint8 receiveNewPass(void)
{
	uint8 newPass[2][NUMOf_PASS_DIGITS];
	uint8 passDigit,inedx;

/* This function check the password and the confirmed
 * if true save the password in EEPROM and send Send CONFIRMED Message to MC1 and return TRUE
 * if false send an error message to MC1 send Send FAILED Message to MC1 and return FALSE
 */

	/*
	 * These two loops are responsible save the receive Byte form MC1 in 2D Array.
	 * newPass[0] save the new Password inputs & newPass[1] save the confirmed password inputs.
	 */
	for(inedx=0; inedx<2; inedx++) {

		for(passDigit=0; passDigit<NUMOf_PASS_DIGITS; passDigit++){
			newPass[inedx][passDigit] =	UART_recieveByte();
		}
	}

	/* call comparePass to check new password and confirmed password are the same or not.
	 * the function return "TRUE" if they are the same and "FALSE" if they NOT.
	 */
	inedx = comparePass(newPass);

	if(inedx == TRUE) {
		/* Write the password in the external EEPROM */
		for(passDigit=0; passDigit<NUMOf_PASS_DIGITS; passDigit++) {
			_delay_ms(30);
			EEPROM_writeByte((firstPass_location+(uint16)passDigit),newPass[0][passDigit]);
		}

		/* send success message to MC1 */
		UART_sendByte(CONFIRMED);
		return TRUE;

	}else if(inedx == FALSE) {
		/* send error message to MC1 */
		UART_sendByte(FAILED);
		return FALSE;
	}
}

void paswordNew(void)
{
	uint8 check;

	while(1) {
		/*
		 * this loop check the return value from receiveNewPass function
		 * if password is saved in EEPROM it will return TRUE then end the loop
		 * if password is not confirmed right it will return FALSE and loop again to retry
		 */

		/* Wait until MC1 is ready to receive
		 * "this technique make UART send and receive at the same time with another MCU"
		 */
		while(UART_recieveByte() != MC1_READY);

		/* get status of saving user inputs.
		 * if password confirmed receiveNewPass function will return "TRUE".
		 * if password not confirmed receiveNewPass function will return "FALSE".
		 */
		check = receiveNewPass();

		if(check == TRUE) {
			break;
		}else if(check == FALSE) {
			/* do nothing */
		}
	}

}


static uint8 securityPass(void)
{
	uint8 entryPass[2][5];
	uint8 passDigit,x;

	/*
	 * This function is get security password inputs from user.
	 * return password status (Right Or Wrong password) by call comparePass function
	 */

	/* Wait until MC1 is ready to receive
     * "this technique make UART send and receive at the same time with another MCU"
	 */
	while(UART_recieveByte() != MC1_READY);

	for(passDigit=0; passDigit<NUMOf_PASS_DIGITS; passDigit++) {
		/* get user inputs and store it in entryPass[0][passDigit] array*/
		entryPass[0][passDigit] = UART_recieveByte();
	}

	for(passDigit=0; passDigit<NUMOf_PASS_DIGITS; passDigit++) {
		/* read the system password from EEPROM and store it in entryPass[1][passDigit] array */
		EEPROM_readByte((firstPass_location+(uint16)passDigit),&x);
		entryPass[1][passDigit] = x;
		_delay_ms(30);
	}

	return comparePass(entryPass); /* return user password inputs status */
}

void makeAction(uint8 action)
{
	uint8 count=0,status;

	/*
	 * This function is for Change Password OR Open Door.
	 * It call securityPass function to get user password input and return either right or wrong.
	 * if password is right call paswordNew OR openDoor function to make new password or Open Door and send password confirmed
	 * message to MC1.
	 * if password is wrong it will send FAILED, then loop another iteration to call
	 * securityPass to get the user password input and check it again.
	 * In case of enter password wrong for 3 times Warning message will display
	 */
	while(1)
	{
		status = securityPass();

		/* Wait until MC1 is ready to receive
		 * "this technique make UART send and receive at the same time with another MCU"
		 */
		while(UART_recieveByte() != MC1_READY);

		if(status == TRUE){
			UART_sendByte(CONFIRMED);

			switch(action)
			{
				case '+':
				openDoor();
				break;

				case '-':
				paswordNew();
				break;

				default: { }
			}
			break; /* exit from the loop and end the function */

		}else if(status == FALSE){
			UART_sendByte(FAILED);
			count++;
			if(count == 3){
				warning();
				count = 0;
				break; /* exit from the loop and end the function */
			}
		}
	}
}

static void buzzerCount(void)
{
	/* Description:
     * For clock=8Mhz and prescale F_CPU/1024 every count will take (0.128)ms
	 * so every overflow match it will take (33)ms
	 * so we need timer to make 1830 overflow matches to get a 60 seconds
	 */

	g_tick++;

	/* 1830 = 60 sec */
	if(g_tick == 1830){
		g_flag = TRUE;
		g_tick = 0;
	}

}


static void warning(void)
{
	timer_config configtimer = {OVERFLOW,DISCONNECTED,CLK_1024,0,0};

	/* configure timer0 */
	timer0_setCallBack(buzzerCount);
	timer0_init(&configtimer);

	while(g_flag != TRUE)
	{
		buzzer_ON();
	}

	buzzer_OFF();
	/* disable timer0 */
	timer0_deinit();
	/* make g_flag FALSE to use it again */
	g_flag = FALSE;
}

static void doorCount(void)
{
	/* Description:
	 * For clock=8Mhz and prescale F_CPU/1024 every count will take (0.128)ms
	 * so every overflow match it will take (33)ms
	 * so we need timer to make 458 overflow matches to get a 15 seconds
	 * , 550 for 18sec, 1007 for 33sec.
	 */

	 g_tick++;

/*This condition indicate that timer count (15)sec, So Turn OFF Motor for the next (3)sec to hold Door*/
	 if(g_tick == 458){
		 dcMotor1SetDirection(STOP,MOTOR_OFF);
/*This condition indicate that timer count (18)sec, So Turn ON Motor for the next (15)sec to close door*/
	 }else if(g_tick == 650){
		 dcMotor1SetDirection(ANTI_CLOCK_WISE,SPEED50);
/*This condition indicate that timer count (33)sec which means Door is closed */
	 }else if(g_tick == 1207){
		dcMotor1_deinit();
		g_flag = TRUE;
		g_tick = 0;
	 }
}

static void openDoor(void)
{
	timer_config configtimer = {OVERFLOW,DISCONNECTED,CLK_1024,0,0};

	dcMotor1_init();

	/* configure timer0 */
	timer0_setCallBack(doorCount);
	timer0_init(&configtimer);

	/* Turn On Motor at the first for (15)sec to Open door */
	dcMotor1SetDirection(CLOCK_WISE,SPEED50);

	while(g_flag != TRUE);

	/* make g_flag FALSE to use it again */
	g_flag = FALSE;

	timer0_deinit();
}
