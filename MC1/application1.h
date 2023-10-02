/*
 * application.h
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#ifndef APPLICATION1_H_
#define APPLICATION1_H_

#include "data_types.h"


/*******************************************************************************
							*      Definitions      *
 *******************************************************************************/

#define  NUMOf_PASS_DIGITS   5

#define  CONFIRMED		     0x36
#define  FAILED		   		 0x63

#define  MC1_READY           0X52
#define  MC2_READY           0X25

#define DOOR_OPENING         0x11
#define DOOR_OPEN        	 0x22
#define DOOR_CLOSING       	 0x33


/*******************************************************************************
					    *      Functions Prototypes      *
 *******************************************************************************/

/*
 * Description:
 * Display the main menu on LCD.
 */
void mainstatus(void);

/*
 * Description:
 * Interact with User when make new Password.
 * get pressed key by keypad and send it to MC2 by UART.
 */
static void newPassword(void);

/*
 * Description:
 * This function is called once in first enter to the system.
 * it ask the user to make password for the system.
 */
void makePasword(void);

/*
 * Description:
 * This function is responsible for change password.
 * Ask the user to enter the system password then change it if right.
 * Display Warning Message in case the user write wrong password for 3 times.
 */
void makeAction(uint8 action);

/*
 * Description:
 * Get the system password from keypad then send the pressed keys to MC2.
 */
static void getSecPass(void);

/*
 * Description:
 * Display first enter to the systems messages.
 */
void firstEnter(void);

/*
 * Description:
 * Display Warning message if security Password entered incorrect for 3 times.
 * Initialize the timer0 to make the message displaying for (1)min.
 */
static void warning(void);

/*
 * Description:
 * function that pass to the call back function.
 * count the required over flow matches for (1)min.
 */
static void warningCount(void);

/*
 * Description:
 * Function to do open Door task.
 * Display Door status on LCD.
 * Initialize the timer0 to count the required time for open and close the door.
 */
static void openDoor(void);

/*
 * Description:
 * function that pass to the call back function.
 * count the required over flow matches.
 */
static void doorCount(void);

#endif /* APPLICATION1_H_ */
