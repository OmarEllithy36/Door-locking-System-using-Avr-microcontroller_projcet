/*
 * gpio.c
 *
 *  Created on: ???/???/????
 *      Author: Omar
 */

#include "gpio.h"
#include "atmega16_reg.h"
#include "bit_math.h"


/*
 * Description :
 * Setup the direction of the required pin input/output.
 * If the input port number or pin number are not correct, The function will not handle the request.
 */
void GPIO_setupPinDirection(uint8 port_num,uint8 pin_num,GPIO_PinDirectionType direction)
{

	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS)){

		/* Do Nothing */

	}else{

		/* Setup the pin direction as required */
		switch (port_num)
		{
		case PORTA_ID:
			if (direction == PIN_INPUT) {
				CLEAR_BIT(DDRA_REG, pin_num);
			} else if (direction == PIN_OUTPUT) {
				SET_BIT(DDRA_REG, pin_num);
			}
			break;

		case PORTB_ID:
			if (direction == PIN_INPUT) {
				CLEAR_BIT(DDRB_REG, pin_num);
			} else if (direction == PIN_OUTPUT) {
				SET_BIT(DDRB_REG, pin_num);
			}
			break;

		case PORTC_ID:
			if (direction == PIN_INPUT) {
				CLEAR_BIT(DDRC_REG, pin_num);
			} else if (direction == PIN_OUTPUT) {
				SET_BIT(DDRC_REG, pin_num);
			}
			break;

		case PORTD_ID:
			if (direction == PIN_INPUT) {
				CLEAR_BIT(DDRD_REG, pin_num);
			} else if (direction == PIN_OUTPUT) {
				SET_BIT(DDRD_REG, pin_num);
			}
			break;
		}
	}
}

/*
 * Description :
 * Write the value Logic High or Logic Low on the required pin.
 * If the input port number or pin number are not correct, The function will not handle the request.
 * If the pin is input, this function will enable/disable the internal pull-up resistor.
 */
void GPIO_writePin(uint8 port_num,uint8 pin_num,uint8 value)
{
	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS)){

		/* Do Nothing */
	}else{

		/* Write the pin value as required */
		switch(port_num)
		{
		case PORTA_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTA_REG,pin_num);
			}else if(value == LOGIC_LOW){
				CLEAR_BIT(PORTA_REG,pin_num);
			}
			break;

		case PORTB_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTB_REG,pin_num);
			}else if(value == LOGIC_LOW){
				CLEAR_BIT(PORTB_REG,pin_num);
			}
			break;

		case PORTC_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTC_REG,pin_num);
			}else if(value == LOGIC_LOW){
				CLEAR_BIT(PORTC_REG,pin_num);
			}
			break;

		case PORTD_ID:
			if(value == LOGIC_HIGH){
				SET_BIT(PORTD_REG,pin_num);
			}else if(value == LOGIC_LOW){
				CLEAR_BIT(PORTD_REG,pin_num);
			}
			break;
		}
	}
}


/*
* Description :
* Read and return the value for the required pin, it should be Logic High or Logic Low.
* If the input port number or pin number are not correct, The function will return Logic Low.
*/
uint8 GPIO_readPin(uint8 port_num,uint8 pin_num)
{
	uint8 pin_value = LOGIC_LOW;

	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS)){

			/* Do Nothing */
	}else{

		/* Read the pin value as required */
		switch(port_num)
		{
		case PORTA_ID:
			pin_value = GET_BIT(PINA_REG, pin_num);
			break;
		case PORTB_ID:
			pin_value = GET_BIT(PINB_REG, pin_num);
			break;
		case PORTC_ID:
			pin_value = GET_BIT(PINC_REG, pin_num);
			break;
		case PORTD_ID:
			pin_value = GET_BIT(PIND_REG, pin_num);
			break;

		}
	}

	return pin_value;
}

/*
 * Description :
 * Setup the direction of the required port all pins input/output.
 * If the direction value is PORT_INPUT all pins in this port should be input pins.
 * If the direction value is PORT_OUTPUT all pins in this port should be output pins.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_setupPortDirection(uint8 port_num,GPIO_PortDirectionType direction)
 {
	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 * In this case the input is not valid port number
	 */
	if(port_num >= NUM_OF_PORTS){

		/* Do Nothing */
	}else{

		/* Setup the port direction as required */
		switch(port_num)
		{
		case PORTA_ID:
			DDRA_REG = direction;
			break;
		case PORTB_ID:
			DDRB_REG = direction;
			break;
		case PORTC_ID:
			DDRC_REG = direction;
			break;
		case PORTD_ID:
			DDRD_REG = direction;
			break;
		}
	}
}

/*
 * Description :
 * Write the value on the required port.
 * If any pin in the port is output pin the value will be written.
 * If any pin in the port is input pin this will activate/deactivate the internal pull-up resistor.
 * If the input port number is not correct, The function will not handle the request.
 */
void GPIO_writePort(uint8 port_num, uint8 value)
{
	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 * In this case the input is not valid port number
	 */
	if(port_num >= NUM_OF_PORTS){

		/* Do Nothing */
	}else{

		/* Write the port value as required */
		switch(port_num)
		{
		case PORTA_ID:
			PORTA_REG = value;
			break;
		case PORTB_ID:
			PORTB_REG = value;
			break;
		case PORTC_ID:
			PORTC_REG = value;
			break;
		case PORTD_ID:
			PORTD_REG = value;
			break;
		}
	}
}

/*
 * Description :
 * Read and return the value of the required port.
 * If the input port number is not correct, The function will return ZERO value.
 */
uint8 GPIO_readPort(uint8 port_num)
{
	uint8 port_value = LOGIC_LOW;

	/*
	 * Check if the input number is greater than NUM_OF_PORTS value.
	 * In this case the input is not valid port number
	 */
	if(port_num >= NUM_OF_PORTS){

		/* Do Nothing */
	}else{

		/* Read the port value as required */
		switch(port_num)
		{
		case PORTA_ID:
			port_value = PINA_REG;
			break;
		case PORTB_ID:
			port_value = PINB_REG;
			break;
		case PORTC_ID:
			port_value = PINC_REG;
			break;
		case PORTD_ID:
			port_value = PIND_REG;
			break;
		}
	}

	return port_value;
}
