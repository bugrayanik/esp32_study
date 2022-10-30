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


void writeTask(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t )pvParam;
	BaseType_t xStatus;

	int i=1;

	while (1)
	{
		xStatus = xQueueOverwrite(QHandle, &i);
		if(xStatus != pdPASS)
			printf("writeTask can't send!\n");
		else
			printf("writeTask send done!\n");

		vTaskDelay(1000 / portTICK_PERIOD_MS);

		i++;
		if(i == 6)
			i = 1;
	}
}



void readTask1(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t )pvParam;
	BaseType_t xStatus;

	int j=0;

	while (1)
	{
		xStatus = xQueuePeek(QHandle, &j,portMAX_DELAY);
		if(xStatus != pdPASS)
			printf("readTask1 can't send!\n");
		else
			printf("readTask1 j=%d!\n",j);

		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}

void readTask2(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t )pvParam;
	BaseType_t xStatus;

	int j=0;

	while (1)
	{
		xStatus = xQueuePeek(QHandle, &j,portMAX_DELAY);
		if(xStatus != pdPASS)
			printf("readTask2 can't send!\n");
		else
			printf("readTask2 j=%d!\n", j);

		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}

void readTask3(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t )pvParam;
	BaseType_t xStatus;

	int j=0;

	while (1)
	{
		xStatus = xQueuePeek(QHandle, &j,portMAX_DELAY);
		if(xStatus != pdPASS)
			printf("readTask3 can't send!\n");
		else
			printf("readTask3 j=%d!\n", j);

		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}


void app_main(void)
{
	QueueHandle_t MailBoxHandle = xQueueCreate(1, sizeof(int));


	if (MailBoxHandle != NULL)
	{
		printf("The queue create successfully!\n");
		xTaskCreate(writeTask, "writeTask", 1024*5, (void *)MailBoxHandle, 1, NULL);
		xTaskCreate(readTask1, "readTask1", 1024*5, (void *)MailBoxHandle, 1, NULL);
		xTaskCreate(readTask2, "readTask2", 1024*5, (void *)MailBoxHandle, 1, NULL);
		xTaskCreate(readTask3, "readTask3", 1024*5, (void *)MailBoxHandle, 1, NULL);
	}
	else
	{
		printf("The queue can't be created\n");
	}

}
