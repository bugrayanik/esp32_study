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

SemaphoreHandle_t SepHandle = NULL;

void myTask1(void *pvParam)
{
	while(1)
	{
		xSemaphoreTake(SepHandle, portMAX_DELAY);
		for(int i=0; i<10; i++)
		{
			printf("In myTask1, i=%d \n", i);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
		xSemaphoreGive(SepHandle);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void myTask2(void *pvParam)
{
	while(1)
	{
		xSemaphoreTake(SepHandle, portMAX_DELAY);
		for(int j=0; j<10; j++)
		{
			printf("In myTask2, j=%d \n", j);
			vTaskDelay(pdMS_TO_TICKS(1000));
		}
		xSemaphoreGive(SepHandle);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


void app_main(void)
{
	SepHandle = xSemaphoreCreateBinary();
	xSemaphoreGive(SepHandle);

	xTaskCreate(myTask1, "myTask1", 1024*5, NULL, 1, NULL);
	xTaskCreate(myTask2, "myTask2", 1024*5, NULL, 1, NULL);

}





