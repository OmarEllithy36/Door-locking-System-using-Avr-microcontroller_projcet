/*
 * buzzer.h
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
#include "data_types.h"

#define  BUZZ_PORT   PORTC_ID
#define  BUZZ_PIN    PIN7_ID

void buzzer_init(void);
void buzzer_ON(void);
void buzzer_OFF(void);


#endif /* BUZZER_H_ */
