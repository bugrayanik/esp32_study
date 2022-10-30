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
	int i = 1;

	while (1)
	{
		/*UBaseType_t iSpaceNum = uxQueueSpacesAvailable(QHandle);
		printf("iSpaceNum = %d!\n", iSpaceNum);
*/
		xStatus = xQueueSendToBack(QHandle, &i, 0);
		if(xStatus != pdPASS)
			printf("can't send!\n");
		else
			printf("send %d!\n", i);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
		i++;
		if(i == 6)
			i = 1;
	}
}

void recTask(void *pvParam)
{
	QueueHandle_t QHandle = (QueueHandle_t ) pvParam;
	BaseType_t xStatus;
	int j = 0;

	while (1)
	{
		UBaseType_t iDataItem = uxQueueMessagesWaiting( QHandle );

		if(iDataItem == 5)
		{
			for (int k=0; k<5; k++)
			{
				xStatus = xQueueReceive(QHandle, &j, 0);
				if(xStatus != pdPASS)
					printf("can't receive!\n");
				else
					printf("received j=%d!\n", j);
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
