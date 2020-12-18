
#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Handy list of GPIO pins on 17xx/40xx boards.
 
 LPCXpresso LPC1769 on base board...
 
 PORT	PIN	Xpresso Pin	Function
 ----	---	-----------	------------------------------------------
 0		17	12			Joystick center button
 0		15	13			Joystick button
 0		16	14			Joystick button
 2		3	45			Joystick button (in towards battery on base board)
 2		4	46			Joystick button (towards closest corner)
 1		31	20			Wakeup button
 0		22	24			LED2 (LED 0 in the software).

 2		0	42			RGB LED RED   PIO1_9
 0		26	18			RGB LED BLUE  PIO1_2
 2		1	43			RGB LED GREEN PIO1_10
 
 Embedded Artists Devkit LPC1788/LPC4088 on base board...
 
 PORT	PIN	DIMM Pin	Function
 ----	---	--------	------------------------------------------
 2		22		120-GPIO73	Joystick center button
 2		23		121-GPIO74	Joystick button
 2		25		122-GPIO75	Joystick button
 2		26		123-GPIO76 	 Joystick button
 2		27  		124-GPIO77	Joystick button
 2		10 		 35-GPIO10	Interrupt button SW6
 */

/* GPIO pin for interrupt */
//#define GPIO_INTERRUPT_PIN     17	/* GPIO pin number mapped to interrupt */
//#define GPIO_INTERRUPT_PORT    GPIOINT_PORT0	/* GPIO port number mapped to interrupt */
#define GPIO_INTERRUPT_PIN_A	 6
#define GPIO_INTERRUPT_PORT_A	 GPIOINT_PORT2

#define GPIO_INTERRUPT_PIN_B	 7
#define GPIO_INTERRUPT_PORT_B	 GPIOINT_PORT2

/* On the LPC1769, the GPIO interrupts share the EINT3 vector. */
#define GPIO_IRQ_HANDLER  		EINT3_IRQHandler/* GPIO interrupt IRQ function name */
#define GPIO_INTERRUPT_NVIC_NAME    	EINT3_IRQn	/* GPIO interrupt NVIC interrupt name */

int n = 12500;

void GPIO_IRQ_HANDLER(void) {
	/* disable interrupt, clear status */
	NVIC_DisableIRQ(GPIO_INTERRUPT_NVIC_NAME);  // prevents interrupt nesting
	Chip_GPIOINT_ClearIntStatus(LPC_GPIOINT, GPIO_INTERRUPT_PORT_A,
			1 << GPIO_INTERRUPT_PIN_A);
while(1)
{
	bool Button_State_1 = Chip_GPIO_GetPinState(LPC_GPIO, GPIO_INTERRUPT_PORT_A,
			GPIO_INTERRUPT_PIN_A);

	if (Button_State_1 == false) {
		n=n-1000;
		Board_LED_Set(2, false);
		delay(n);
		Board_LED_Set(2, true);
		delay(n);
	}
}
	/* re-enable interrupt */
	NVIC_EnableIRQ(GPIO_INTERRUPT_NVIC_NAME);
}

void delay(int num) {
	for (int i = 0; i <= num; i++) {
		for (int j = 0; j <= 1000; j++)
			;
	}
}
/**
 * @brief	Main program body
 * @return	Does not return
 */
int main(void) {
	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_LED_Set(0, false);
	Board_LED_Set(1, false);
	Board_LED_Set(2, false);
	/* Board_Init calls Chip_GPIO_Init and enables GPIO clock if needed,
	 Chip_GPIO_Init is not called again */
	Board_Init();

	Board_LED_Set(2, true);

	/* Configure GPIO interrupt pin as input */
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_INTERRUPT_PORT_A,
			GPIO_INTERRUPT_PIN_A);

	/* Configure the GPIO interrupt */
	Chip_GPIOINT_SetIntFalling(LPC_GPIOINT, GPIO_INTERRUPT_PORT_A,
			1 << GPIO_INTERRUPT_PIN_A);

	/* Enable interrupt in the NVIC */
	NVIC_ClearPendingIRQ(GPIO_INTERRUPT_NVIC_NAME);
	NVIC_EnableIRQ(GPIO_INTERRUPT_NVIC_NAME);

	/* Wait for interrupts - LED will toggle on each wakeup event */
	while (1) {
		__WFI();
	}

	return 0;
}

