#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "addr_from_stdin.h"
#include "lwip/err.h"
#include "lwip/sockets.h"


static const char *TAG = "example";
static const char *payload = "Message from ESP32";

static void tcp_client_task(void *pvParameters)
{
	char rx_buffer[128];
    char host_ip[] = "10.42.0.97";
    int addr_family = 0;
    int ip_protocol = 0;
    int PORT = 2223;
    struct sockaddr_in dest_addr;
    
    dest_addr.sin_addr.s_addr = inet_addr("10.42.0.97");
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(2223);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;
    
	

	while(1){
		
		vTaskDelay(2000 / portTICK_PERIOD_MS);
		int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
		if (sock <= 0) 
		{
	        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
	        while(1)
	        {
	        	int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
	        	vTaskDelay(5000 / portTICK_PERIOD_MS);
	        	if(sock > 0 )
	        	{
	        		ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);
	        		break;
				}
				
			
			}
		}
		ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);
		
		
		
		int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
		if (err < 0) 
		{
			ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);  
			shutdown(sock,0);
			close(sock);
		}
	else
	{
		ESP_LOGI(TAG, "Successfully connected");
		while (1)
		{
			int err = send(sock, payload, strlen(payload), 0);
			if (err < 0)
			{
				ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
				shutdown(sock,0);
				close(sock);
						
			}
			int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
			if (len < 0)
			{
				ESP_LOGE(TAG, "recv failed: errno %d", errno);
				break;
				
			}
			else
			{
				rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                ESP_LOGI(TAG, "%s", rx_buffer);
			}
			vTaskDelay(2000 / portTICK_PERIOD_MS);
		}
	}
	
	}

	
	vTaskDelete(NULL);
}
	
void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());
    
    xTaskCreate(tcp_client_task, "tcp_client", 4096, NULL, 5, NULL);
}
