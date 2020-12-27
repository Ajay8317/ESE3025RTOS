/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
/*declare a structure for storing delay and led number*/
struct datta {
	int del;
	int led;
} data[3];
/* setup system hardware */
static void prvSetupHardware(void) {
	SystemCoreClockUpdate();
	Board_Init();
	/* Initial LED0 state is off */
	Board_LED_Set(0, true);
	Board_LED_Set(1, true);
	Board_LED_Set(2, true);
}
/*led task*/
static void vLEDTask2(void *pvParameters) {
	/*cast the void pointer*/
	struct datta *dd = pvParameters;
	bool LedState = false;
	vTaskDelay((*dd).del);
	while (1) {
		Board_LED_Set((*dd).led, false);
		vTaskDelay(1000);
		Board_LED_Set((*dd).led, true);
		vTaskDelay(3500);
	}
}
int main(void) {
	data[0].del = 0;
	data[0].led = 0;
	data[1].del = 1500;
	data[1].led = 1;
	data[2].del = 3000;
	data[2].led = 2;
	prvSetupHardware();
	xTaskCreate(vLEDTask2, (signed char* ) "vTaskLed1",
			configMINIMAL_STACK_SIZE, &data[0], (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);
	xTaskCreate(vLEDTask2, (signed char* ) "vTaskLed2",
			configMINIMAL_STACK_SIZE, &data[1], (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);
	xTaskCreate(vLEDTask2, (signed char* ) "vTaskLed3",
			configMINIMAL_STACK_SIZE, &data[2], (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);
	vTaskStartScheduler();
	/* Should never arrive here */
	return 1;
}
