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
#include "freertos/semphr.h"

SemaphoreHandle_t MutexHandle = NULL;

void Task1(void *pvParam)
{
	int iPriority = 0;
	iPriority = uxTaskPriorityGet(NULL);
	printf("Task1 begin: iPriority = %d \n", iPriority);

	while(1)
	{
		xSemaphoreTake(MutexHandle, portMAX_DELAY);
		iPriority = uxTaskPriorityGet(NULL);
		printf("Task1 take: iPriority = %d \n", iPriority);

		for(int i=0; i<10; i++) //to simulate data progressing
		{
			iPriority = uxTaskPriorityGet(NULL);
			printf("Task1 i=%d, iPriority = %d \n",i, iPriority);
			vTaskDelay(pdMS_TO_TICKS(1000));

		}

		xSemaphoreGive(MutexHandle);
		iPriority = uxTaskPriorityGet(NULL);
		printf("Task1 give: iPriority = %d \n", iPriority);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void Task2(void *pvParam)
{
	vTaskDelay(pdMS_TO_TICKS(1000));

	int iPriority = 0;
	iPriority = uxTaskPriorityGet(NULL);
	printf("Task2 begin: iPriority = %d \n", iPriority);

	while(1)
	{
		xSemaphoreTake(MutexHandle, portMAX_DELAY);
		iPriority = uxTaskPriorityGet(NULL);
		printf("Task2 take: iPriority = %d \n", iPriority);

		for(int i=0; i<10; i++) //to simulate data progressing
		{
			iPriority = uxTaskPriorityGet(NULL);
			printf("Task2 i=%d, iPriority = %d \n",i, iPriority);
			vTaskDelay(pdMS_TO_TICKS(1000));

		}

		xSemaphoreGive(MutexHandle);
		iPriority = uxTaskPriorityGet(NULL);
		printf("Task2 give: iPriority = %d \n", iPriority);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


void app_main(void)
{
	MutexHandle = xSemaphoreCreateMutex();

	if(MutexHandle != NULL)
	{
		xTaskCreate(Task1, "Task1", 1024*5, NULL, 1, NULL);
		xTaskCreate(Task2, "Task2", 1024*5, NULL, 2, NULL);
	}
}
