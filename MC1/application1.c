/*
 * application.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#include "application1.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "util/delay.h"
#include "timer.h"



/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

static uint16 g_tick = 0;
static uint8 g_flag = FALSE;
static uint8 g_door = 0;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void firstEnter(void)
{
	/* display messages for the first enter */
		LCD_clearScreen();
		LCD_moveCursor(0,4);
		LCD_displayString("WELCOME");
		_delay_ms(1500);
		LCD_clearScreen();
		LCD_displayString("Creating Your");
		LCD_moveCursor(1,0);
		LCD_displayString("Password...");
		_delay_ms(2000);
}
void mainstatus(void)
{
	/* display messages for the Main Status mode */
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString("+:Open Door ");
	LCD_moveCursor(1,0);
	LCD_displayString("-:Change Password");
}


static void newPassword(void)
{
	uint8 pressedKey,count=0,col=10;

	/* Ask user to Enter New Password */
	LCD_clearScreen();
	LCD_displayString("Enter New ");
	LCD_moveCursor(1,0);
	LCD_displayString("Password:");

	/* this loop responsible for:
	 * interact with the user by LCD and send password to MC2 from user 2 times
	 * first time to send new password
	 * second time to send confirmed password
	 * it loops for 10 iterations, 5 for new password and 5 for confirmation
	 */
	while(count < (2*NUMOf_PASS_DIGITS)){
		pressedKey = KEYPAD_getPressedKey();

		/* Send the pressed key to MC2 through UART */
		UART_sendByte(pressedKey);

		/* write "*" for every pressed key */
		LCD_moveCursor(1,col);
		LCD_displayCharacter('*');
		count++;
		col++;  /* increment col to jump to the next column in LCD */

	/* this condition indicate that the new password is sent then ask user to write confirm password */
		if(count == NUMOf_PASS_DIGITS){
			_delay_ms(200); /* wait few ms second to make human Eye notice the previous "*" */
			LCD_clearScreen();
			LCD_displayString("Confirm New ");
			LCD_moveCursor(1,0);
			LCD_displayString("Password:");
			col = 10;  /* assign col=10 to write from the 10th column again */
		}
		_delay_ms(400); /* wait few ms second to prevent multiple keypad press */
	}
}


void makePasword(void)
{
	uint8 test;

	while(1) {
		/*
		 * call function newPassword to get password from user and send it to MC2
		 * this loop check the receive message from MC2 after send user's keys pressed
		 * if password is saved in EEPROM confirmed message will receive then end the loop
		 * if password is not confirmed right it will receive failed message
		 * In case of Failed ask the user to retry and loop another iteration
		 */

		/* Send MC1_SEND_READY byte to MC2 to inform that MC1 will send
		 * this technique make UART send and receive at the same time with another MCU
		 */
		UART_sendByte(MC1_READY);

		/* call this function to get pressed key by keypad and send it to MC2 by UART */
		newPassword();

		/* receive password saving status from MC2 */
		test = UART_recieveByte();

		if (test == CONFIRMED) {
			LCD_clearScreen();
			LCD_displayString("Password Saved !");
			_delay_ms(1000);  /* wait few ms second to make human Eye notice the message */
			break;
		} else if (test == FAILED) {
			LCD_clearScreen();
			LCD_moveCursor(0, 4);
			LCD_displayString("FAILED");
			_delay_ms(1000);  /* wait few ms second to make human Eye notice the message */
		}
	}
}

void makeAction(uint8 action)
{
	uint8 status, count = 0;

	/*
	 * this loop responsible for:
	 * call function getSecPass to get password from user and send it to MC2.
	 * this loop check the receive message from MC2 after send user's keys pressed.
	 * if password is right CONFIRMED message will receive, then ask user to
	 * make new password by call makePassword function.
	 * if password is wrong it will receive FAILED, then ask the user to
	 * retry and loop another iteration.
	 * In case of enter password wrong for 3 times Warning message will display
	 */
	while(1) {
		/* call this function to interface with user and send the user password inputs to MC2 */
		getSecPass();

		/* Send MC1_READY byte to MC2 to inform that MC1 ready to receive
		 * "this technique make UART send and receive at the same time with another MCU"
		 */
		UART_sendByte(MC1_READY);

		/* receive password checking status from MC2 */
		status = UART_recieveByte();

		if(status == CONFIRMED) {
			switch(action)
			{
				case '+':
				openDoor();
				break;

				case '-':
				makePasword();
				LCD_clearScreen();
				LCD_displayString("Change Done");
				_delay_ms(1000);
				break;

				default: { }
			}
			break;  /* exit from the loop and end the function */

		}else if(status == FAILED) {
			LCD_clearScreen();
			LCD_displayString("Incorrect");
			LCD_moveCursor(1, 0);
			LCD_displayString("Password");
			_delay_ms(1500);
			count++;
			if(count == 3) {
				warning();
				count = 0;
				break;
			}
		}

	}

}

static void getSecPass(void)
{
	uint8 count,keyPresed;

	LCD_clearScreen();
	LCD_displayString("Enter Password:");

	/* Send MC1_SEND_READY byte to MC2 to inform that MC1 will send
	 * this technique make UART send and receive at the same time with another MCU
	 */
	UART_sendByte(MC1_READY);

	/* loop to get pressed keys then send it to MC2 */
	for(count=0; count<NUMOf_PASS_DIGITS; count++) {
		_delay_ms(400);
		keyPresed = KEYPAD_getPressedKey();

		/* write "*" for every pressed key */
		LCD_moveCursor(1,count);
		LCD_displayCharacter('*');
/* warning */
		UART_sendByte(keyPresed);
	}
	_delay_ms(300); /* wait few ms second to make human Eye notice the previous "*" */
}

static void warningCount(void)
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
	timer0_setCallBack(warningCount);
	timer0_init(&configtimer);

	while(g_flag != TRUE)
	{
		LCD_clearScreen();
		_delay_ms(400);
		LCD_moveCursor(0,3);
		LCD_displayString("WARNING !!");
		_delay_ms(400);
	}

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

	 /* 458 = 15sec  */
	 if(g_tick == 458){
	   	g_door = DOOR_OPEN;
	  /* 650 = 15+3 sec  */
	 }else if(g_tick == 650){
		g_door = DOOR_CLOSING;
	/* 1207 = 15+3+15 sec */
	 }else if(g_tick == 1207){
		g_flag = TRUE;
		g_tick = 0;
		g_door = 0;
	 }
}


static void openDoor(void)
{
	timer_config configtimer = {OVERFLOW,DISCONNECTED,CLK_1024,0,0};

	/* configure timer0 */
	timer0_setCallBack(doorCount);
	timer0_init(&configtimer);

	/* assign g_door to DOOR_OPENING to start display open door phase */
	g_door = DOOR_OPENING;

	while(g_flag != TRUE)
	{
		while(g_door == DOOR_OPENING){
			LCD_clearScreen();
			_delay_ms(400);
			LCD_moveCursor(0,2);
			LCD_displayString("Door Opening");
			_delay_ms(400);
		}

		while(g_door == DOOR_OPEN){
			LCD_moveCursor(0,1);
			LCD_displayString("Door Is Opened");
		}

		while(g_door == DOOR_CLOSING){
			LCD_clearScreen();
			_delay_ms(400);
			LCD_moveCursor(0,2);
			LCD_displayString("Door Closing");
			_delay_ms(400);
		}
	}

	/* make g_flag FALSE to use it again */
	g_flag = FALSE;
	/* disable timer0 */
	timer0_deinit();
}
