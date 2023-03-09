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
#include "freertos/event_groups.h"

EventGroupHandle_t EventGroupHandle = NULL;

#define BIT_0 ( 1 << 0 )
#define BIT_4 ( 1 << 4 )

void Task1(void *pvParam)
{
	while(1)
	{
		printf("--------------------\n");
		printf("task1 begin to check!\n");
		xEventGroupWaitBits(
				EventGroupHandle,
				BIT_0 | BIT_4,
				pdTRUE,
				pdTRUE, //pdFALSE,
				portMAX_DELAY);
		printf("In task1, BIT_0 and BIT_4 is set!\n"); //printf("In task1, BIT_0 or BIT_4 is set!\n");
		vTaskDelay(pdMS_TO_TICKS(1000));

	}
}

void Task2(void *pvParam)
{
	vTaskDelay(pdMS_TO_TICKS(1000));

	while(1)
	{
		printf("--------------------\n");
		printf("task2 begin to set BIT_O!\n");
		xEventGroupSetBits( EventGroupHandle,
				BIT_0);

		vTaskDelay(pdMS_TO_TICKS(5000));

		printf("--------------------\n");
		printf("task2 begin to set BIT_4!\n");
		xEventGroupSetBits( EventGroupHandle,
				BIT_4);

		vTaskDelay(pdMS_TO_TICKS(5000));
	}

}


void app_main(void)
{
	EventGroupHandle = xEventGroupCreate();

	if(EventGroupHandle != NULL)
	{
		xTaskCreate(Task1, "Task1", 1024*5, NULL, 1, NULL);
		xTaskCreate(Task2, "Task2", 1024*5, NULL, 1, NULL);
	}
}
