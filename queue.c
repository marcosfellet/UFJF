#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/queue.h>
#include "dht11.h"



#define BTIO GPIO_NUM_2
#define DHT11_PIN GPIO_NUM_4

QueueHandle_t queue;

struct dht11_reading data;


void vtaskBT(void *pvparameters)
{
    ESP_LOGI("BT", "task inicializando");
    while(1)
    {
        if(gpio_get_level(BTIO) == 0)
        {
            ESP_LOGI("BT", "Botao pressionado");
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        
    }
}

void vtaskRD(void *pvparameters)
{
    ESP_LOGI("RD", "task inicializando");
    struct dht11_reading data;
    while(1)
    {
        data = DHT11_read();
        xQueueSendToBack(queue, &data, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void vtaskDP(void *pvparameters)
{
    ESP_LOGI("DP", "task inicializando");
    struct dht11_reading data_display;
    while(1)
    {
        xQueueReceive(queue, &data_display, portMAX_DELAY);
        ESP_LOGI("DP", "Umidade (%%): %f", (float)data_display.humidity);
        ESP_LOGI("DP", "Temperatura (°C): %f", (float)data_display.temperature);
    }
}

void app_main(void)
{
vTaskPrioritySet(NULL, 5);
gpio_set_pull_mode(DHT11_PIN, GPIO_PULLUP_ONLY);

DHT11_init(DHT11_PIN);

gpio_reset_pin(BTIO);
gpio_set_direction(BTIO, GPIO_MODE_INPUT);
gpio_set_pull_mode(BTIO, GPIO_PULLUP_ONLY);

queue = xQueueCreate(10, sizeof(data));

xTaskCreate(vtaskBT, "TASK BT", 2048, NULL, 4, NULL);
xTaskCreate(vtaskRD, "TASK RD", 2048, NULL, 3, NULL);
xTaskCreate(vtaskDP, "TASK DP", 2048, NULL, 2, NULL);

}
