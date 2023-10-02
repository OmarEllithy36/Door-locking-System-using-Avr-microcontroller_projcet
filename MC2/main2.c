/*
 * main2.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */


/*
 * main22.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

/*
 * main2.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */
#include "data_types.h"
#include "uart.h"
#include "i2c.h"
#include "util/delay.h"
#include "application2.h"
#include "avr/io.h"
#include "dc_motor.h"
#include "buzzer.h"


int main(void)
{
	uint8 MC1Recevie;
	uart_config configUART = {EIGHT_BIT,ONE_BIT,ENABLE_ODD,9600};
	Config_TWI  configtwi = {FAST,0x04};

	SREG |= (1<<7);  /* enable global interrupt bit */

	UART_init(&configUART);
	TWI_init(&configtwi);
	buzzer_init();

/* make system password for the first time */
	paswordNew();

	while(1)
	{
		/* Wait until MC1 is ready to receive
		 * "this technique make UART send and receive at the same time with another MCU"
		 */
		while(UART_recieveByte() != MC1_READY);

		MC1Recevie = UART_recieveByte();

		switch(MC1Recevie)
		{
			case '+':
				makeAction('+');
				break;

			case '-':
				makeAction('-');
				break;

			default: { }
		}

	}

	return 0;
}

