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

typedef struct A_STRUCT
{
	char id;
	char data;
} xStruct;


void sendTask(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t )pvParam;
	BaseType_t xStatus;
	xStruct xUSB = {1, 5};

	while (1)
	{
		/*UBaseType_t iSpaceNum = uxQueueSpacesAvailable(QHandle);
		printf("iSpaceNum = %d!\n", iSpaceNum);
*/
		xStatus = xQueueSendToBack(QHandle, &xUSB, 0);
		if(xStatus != pdPASS)
			printf("can't send!\n");
		else
			printf("send \n");

		vTaskDelay(1000 / portTICK_PERIOD_MS);

		xUSB.id++;
		if(xUSB.id == 6)
			xUSB.id = 1;
	}
}

void recTask(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t ) pvParam;
	BaseType_t xStatus;

	xStruct xUSB = {0, 0};

	while (1)
	{
		UBaseType_t iDataItem = uxQueueMessagesWaiting( QHandle );

		if(iDataItem == 5)
		{
			for (int k=0; k<5; k++)
			{
				xStatus = xQueueReceive(QHandle, &xUSB, 0);
				if(xStatus != pdPASS)
					printf("can't receive!\n");
				else
					printf("received id=%d, data=%d!\n", xUSB.id, xUSB.data);
			}
		}
		/*printf("iDataItem = %d!\n", iDataItem);

		xStatus = xQueueReceive(QHandle, &j, 0);
		if(xStatus != pdPASS)
			printf("can't receive!\n");
		else
			printf("received j=%d!\n", j);*/
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
