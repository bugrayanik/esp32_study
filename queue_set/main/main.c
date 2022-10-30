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


	void sendTask1(void *pvParam)
	{
		QueueHandle_t QHandle = (QueueHandle_t )pvParam;
		BaseType_t xStatus;

		int i=1;

		while (1)
		{
			xStatus = xQueueSendToBack(QHandle, &i, 0);
			if(xStatus != pdPASS)
				printf("task1 can't send!\n");
			else
				printf("task1 send done!\n");

			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	void sendTask2(void *pvParam)
	{
		QueueHandle_t QHandle = (QueueHandle_t )pvParam;
		BaseType_t xStatus;


		int i=2;

		while (1)
		{
			xStatus = xQueueSendToBack(QHandle, &i, 0);
			if(xStatus != pdPASS)
				printf("task2 can't send!\n");
			else
				printf("task2 send done!\n");

			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	}

	void recTask(void *pvParam)
	{
		QueueSetHandle_t QSetHandle = (QueueSetHandle_t ) pvParam;
		BaseType_t xStatus;

		int j = 0;

		while (1)
		{
			QueueSetMemberHandle_t QSetMemHandle = xQueueSelectFromSet(QSetHandle, portMAX_DELAY);
			xStatus = xQueueReceive(QSetMemHandle, &j, portMAX_DELAY);

			if(xStatus != pdPASS)
				printf("receive fail!\n");
			else
				printf("received: %d!\n", j);
		}
	}

	void app_main(void)
	{
		QueueHandle_t QHandle1 = xQueueCreate(5, sizeof(int));
		QueueHandle_t QHandle2 = xQueueCreate(5, sizeof(int));
		QueueSetHandle_t QSetHandle = xQueueCreateSet(10);

		xQueueAddToSet(QHandle1, QSetHandle);
		xQueueAddToSet(QHandle2, QSetHandle);

		if ((QHandle1 != NULL) && (QHandle2 != NULL) && (QSetHandle != NULL))
		{
			printf("The queue create successfully!\n");
			xTaskCreate(sendTask1, "sendTask1", 1024*5, (void *)QHandle1, 1, NULL);
			xTaskCreate(sendTask2, "sendTask2", 1024*5, (void *)QHandle2, 1, NULL);
			xTaskCreate(recTask, "recTask", 1024*5, (void *)QSetHandle, 3, NULL);

		}
		else
		{
			printf("The queue can't be created\n");
		}

	}
