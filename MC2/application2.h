/*
 * function2.h
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#ifndef APPLICATION2_H_
#define APPLICATION2_H_

#include "data_types.h"

/*******************************************************************************
 *                               Definition                                    *
 *******************************************************************************/

#define  NUMOf_PASS_DIGITS	    5

#define  CONFIRMED		        0x36
#define  FAILED		            0x63

#define  MC1_READY              0X52
#define  MC2_READY              0X25

#define MOTOR_CLOCK_WISE     	0x33
#define MOTOR_ANTI_CLOCK_WISE   0x22
#define MOTOR_MOTOR_STOP	    0x11

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Compare between 2D Array.
 * it return TRUE if the two array are the same and FALSE if they are NOT.
 */
static uint8 comparePass(const uint8 password_arr[][5]);

/*
 * Description:
 * Receive new password and its confirmation.
 * Save new Password in External EEPROM if confirmed successfully.
 */
static uint8 receiveNewPass(void);

/*
 * Description:
 * Make new Password for the system.
 */
void paswordNew(void);

/*
 * Description:
 * Get security password inputs from user.
 * return password status (Right Or Wrong password) by call comparePass function.
 */
static uint8 securityPass(void);

/*
 * Description:
 * Do action in the system either Change Password OR Open Door.
 */
void makeAction(uint8 action);

/*
 * Description:
 * Turn on Buzzer if security Password entered incorrect for 3 times.
 * Initialize the timer0 to make Buzzer turn on for (1)min.
 */
static void warning(void);

/*
 * Description:
 * function that pass to the call back function.
 * count the required over flow matches for (1)min.
 */
static void buzzerCount(void);

/*
 * Description:
 * Function to do open Door task.
 * Initialize the timer0 to count the required time for open and close the door.
 * Control the Dc motor to make door open & close.
 */
static void openDoor(void);

/*
 * Description:
 * function that pass to the call back function.
 * Turn Motor ON&OFF and de init with the respect time.
 * count the required over flow matches.
 */
static void doorCount(void);

#endif /* APPLICATION2_H_ */
