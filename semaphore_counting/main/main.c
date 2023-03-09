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

void carInTask(void *pvParam)
{
	int iEmptySpace = 0;
	int iResult = 0;

	while(1)
	{
		iEmptySpace = uxSemaphoreGetCount(SepHandle);
		printf("iEmptySpace = %d \n", iEmptySpace);

		iResult = xSemaphoreTake(SepHandle, 0);
		if(iResult == pdPASS)
			printf("One car in\n");
		else
			printf("No space\n");

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void carOutTask(void *pvParam)
{
	while(1)
	{
		vTaskDelay(pdMS_TO_TICKS(6000));

		xSemaphoreGive(SepHandle);
		printf("One car out\n");
	}
}


void app_main(void)
{
	SepHandle = xSemaphoreCreateCounting(5,5);

	if(SepHandle != NULL)
	{
		xTaskCreate(carInTask, "carInTask", 1024*5, NULL, 1, NULL);
		xTaskCreate(carOutTask, "carOutTask", 1024*5, NULL, 1, NULL);
	}
}





