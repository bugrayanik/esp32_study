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
	printf("MyTimerCallBack!\n");
}


void app_main(void)
{
	TimerHandle_t myTimer = NULL;
	myTimer = xTimerCreate("myTimer", pdMS_TO_TICKS(1000), pdTRUE, ( void * ) 0, myTimerCallBack);

	printf("Timer start!\n");
	xTimerStart(myTimer, 0);

	vTaskDelay(6000 / portTICK_PERIOD_MS);

	xTimerStop(myTimer, 0);
	printf("Timer stop!\n");

}





