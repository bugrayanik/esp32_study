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


void sendTask(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t )pvParam;
	BaseType_t xStatus;
	int i = 0;

	while (1)
	{
		xStatus = xQueueSendToBack(QHandle, &i, 0);
		if(xStatus != pdPASS)
			printf("can't send!\n");
		else
			printf("send done!\n");

		vTaskDelay(1000 / portTICK_PERIOD_MS);
		i++;
	}
}

void recTask(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t ) pvParam;
	BaseType_t xStatus;
	int j = 0;

	while (1)
	{
		xStatus = xQueueReceive(QHandle, &j, 0);
		if(xStatus != pdPASS)
			printf("can't receive!\n");
		else
			printf("received j=%d!\n", j);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}

void app_main(void)
{
	QueueHandle_t QHandle = xQueueCreate(5, sizeof(int));

	if (QHandle != NULL)
	{
		printf("The queue create successfully!\n");
		xTaskCreate(sendTask, "sendTask", 1024*5, (void *)QHandle, 0, NULL);
		xTaskCreate(recTask, "recTask", 1024*5, (void *)QHandle, 0, NULL);

	}
	else
	{
		printf("The queue can't be created\n");
	}

}
