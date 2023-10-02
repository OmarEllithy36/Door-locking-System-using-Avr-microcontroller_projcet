/*
 * main1.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#include "keypad.h"
#include "uart.h"
#include "util/delay.h"
#include "avr/io.h"

#include "application1.h"
#include "lcd.h"


int main(void)
{
	uint8 mainPressed;
	uart_config configUART = {EIGHT_BIT,ONE_BIT,ENABLE_ODD,9600};

	SREG |= (1<<7);  /* enable global interrupt bit */

	LCD_init();
	UART_init(&configUART);

	/*
	 * Call firstEnter to display Welcome message.
	 * Call makePasword to create system password.
	 */
	firstEnter();
	makePasword();


	while(1)
	{
		mainstatus();
		mainPressed = KEYPAD_getPressedKey();

		switch(mainPressed)
		{
			case '+':
				UART_sendByte(MC1_READY);
				_delay_ms(5);
				UART_sendByte('+');
				makeAction('+');
				break;

			case '-':
				UART_sendByte(MC1_READY);
				_delay_ms(5);
				UART_sendByte('-');
				makeAction('-');
				break;

			default: { }

		}

	}

	return 0;
}

