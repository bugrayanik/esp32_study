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
	while(1)
	{
		printf("--------------------\n");
		printf("task1 begin\n");
		xSemaphoreTakeRecursive(MutexHandle, portMAX_DELAY);

		printf("Task1 take A!\n");
		for(int i=0; i<10; i++) //to simulate data progressing
		{
			printf("Task1 i=%d for A !\n",i);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}

		xSemaphoreTakeRecursive(MutexHandle, portMAX_DELAY);

		printf("Task1 take B!\n");
		for(int i=0; i<10; i++) //to simulate data progressing
		{
			printf("Task1 i=%d for B !\n",i);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}

		printf("task1 give B!\n");
		xSemaphoreGiveRecursive(MutexHandle);
		vTaskDelay(pdMS_TO_TICKS(3000));

		printf("task1 give A!\n");
		xSemaphoreGiveRecursive(MutexHandle);
		vTaskDelay(pdMS_TO_TICKS(3000));


	}
}

void Task2(void *pvParam)
{
	vTaskDelay(pdMS_TO_TICKS(1000));

	while(1)
	{
		printf("--------------------\n");
		printf("task2 begin\n");
		xSemaphoreTakeRecursive(MutexHandle, portMAX_DELAY);

		printf("Task2 take A!\n");
		for(int i=0; i<10; i++) //to simulate data progressing
		{
			printf("Task2 i=%d for A !\n",i);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}

		printf("task1 give A!\n");
		xSemaphoreGiveRecursive(MutexHandle);
		vTaskDelay(pdMS_TO_TICKS(3000));
	}

}


void app_main(void)
{
	MutexHandle = xSemaphoreCreateRecursiveMutex();

	if(MutexHandle != NULL)
	{
		xTaskCreate(Task1, "Task1", 1024*5, NULL, 1, NULL);
		xTaskCreate(Task2, "Task2", 1024*5, NULL, 2, NULL);
	}
}
