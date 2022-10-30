/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"

TaskHandle_t myTask2_Handle;


void myTask1(void *pvParam)
{
	while(1)
	{
		printf("myTask1!\n");
		vTaskDelay(3000 / portTICK_PERIOD_MS);

		vTaskSuspend(myTask2_Handle);

		printf("myTask1 collect_data_01\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		printf("myTask1 collect_data_02\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		printf("myTask1 collect_data_03\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);

		vTaskResume(myTask2_Handle);

	}
}

void myTask2(void *pvParam)
{
	while(1)
	{
		printf("myTask2!\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void app_main(void)
{
    xTaskCreate(myTask1, "myTask1", 1024*2, NULL, 1, NULL);
    xTaskCreate(myTask2, "myTask2", 1024*2, NULL, 1, &myTask2_Handle);

    static char pcWriteBuffer[1024] = {0};

    while(1)
    {
    	vTaskList((char *)pcWriteBuffer);
    	printf("Name    State    Priority    Stack    NUM    \r\n");
    	printf("\r\n%s\r\n", pcWriteBuffer);
    	vTaskDelay(5000 / portTICK_PERIOD_MS);

    }

}
