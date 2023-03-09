/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_flash.h"
#include "freertos/queue.h"
#include "freertos/timers.h"

void myTimerCallBack(TimerHandle_t xTimer)
{
	const char *strTimerName;
	strTimerName = pcTimerGetName(xTimer);

	int *id;
	id = (int *) pvTimerGetTimerID(xTimer);
	printf("Timer Name = %s, Timer ID = %d!\n", strTimerName, *id);
}

int id1 = 1;
int id2 = 2;


void app_main(void)
{
	TimerHandle_t myTimer1 = NULL;
	TimerHandle_t myTimer2 = NULL;

	myTimer1 = xTimerCreate("myTimer1", pdMS_TO_TICKS(1000), pdTRUE, ( void * ) &id1, myTimerCallBack);
	myTimer2 = xTimerCreate("myTimer2", pdMS_TO_TICKS(2000), pdTRUE, ( void * ) &id2, myTimerCallBack);

	printf("Timer start!\n");
	xTimerStart(myTimer1, 0);
	xTimerStart(myTimer2, 0);

	vTaskDelay(pdMS_TO_TICKS(5000));

	xTimerChangePeriod(myTimer1, pdMS_TO_TICKS(5000), 0);

/*	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
		xTimerReset(myTimer2, 0);
	}*/

}





