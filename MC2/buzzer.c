/*
 * buzzer.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#include "buzzer.h"


void buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZ_PORT,BUZZ_PIN,PIN_OUTPUT);
	GPIO_writePin(BUZZ_PORT,BUZZ_PIN,LOGIC_LOW);
}

void buzzer_ON(void)
{
	GPIO_writePin(BUZZ_PORT,BUZZ_PIN,LOGIC_HIGH);
}

void buzzer_OFF(void)
{
	GPIO_writePin(BUZZ_PORT,BUZZ_PIN,LOGIC_LOW);
}
