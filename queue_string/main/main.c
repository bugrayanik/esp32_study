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

	char *pcStrToSend;
	const int strMaxLen = 50;
	int i=1;

	while (1)
	{
		/*UBaseType_t iSpaceNum = uxQueueSpacesAvailable(QHandle);
		printf("iSpaceNum = %d!\n", iSpaceNum);
*/
		pcStrToSend = (char *)malloc(strMaxLen);
		snprintf(pcStrToSend, strMaxLen, "String number = %d", i);


		xStatus = xQueueSendToBack(QHandle, &pcStrToSend, 0);
		if(xStatus != pdPASS)
			printf("can't send!\n");
		else
			printf("send \n");

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

	char * pcStrToRec;

	while (1)
	{
		UBaseType_t iDataItem = uxQueueMessagesWaiting( QHandle );

		if(iDataItem == 5)
		{
			for (int k=0; k<5; k++)
			{
				xStatus = xQueueReceive(QHandle, &pcStrToRec, 0);
				if(xStatus != pdPASS)
					printf("can't receive!\n");
				else
				{
					printf("received %s!\n", pcStrToRec);
					free(pcStrToRec);
				}
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
