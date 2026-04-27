#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include "dht11.h"



#define BTIO GPIO_NUM_2
#define DHT11_PIN GPIO_NUM_4

SemaphoreHandle_t sem_bin;
SemaphoreHandle_t sem_bin2;
SemaphoreHandle_t mutex;

struct dht11_reading data;


void vtaskBT(void *pvparameters)
{
    ESP_LOGI("BT", "task inicializando");
    while(1)
    {
        if(gpio_get_level(BTIO) == 0)
        {
            ESP_LOGI("BT", "Botao pressionado");
            xSemaphoreGive(sem_bin);
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        vTaskDelay(pdMS_TO_TICKS(100));
        
    }
}

void vtaskRD(void *pvparameters)
{
    ESP_LOGI("RD", "task inicializando");
    while(1)
    {
        xSemaphoreTake(sem_bin, portMAX_DELAY);
        xSemaphoreTake(mutex, portMAX_DELAY);
        data = DHT11_read();
        vTaskDelay(pdMS_TO_TICKS(50));
        xSemaphoreGive(mutex);
        xSemaphoreGive(sem_bin2);
    }
}

void vtaskDP(void *pvparameters)
{
    ESP_LOGI("DP", "task inicializando");
    while(1)
    {
        xSemaphoreTake(sem_bin2, portMAX_DELAY);
        xSemaphoreTake(mutex, portMAX_DELAY);
        ESP_LOGI("DP", "Umidade (%%): %f", (float)data.humidity);
        ESP_LOGI("DP", "Temperatura (°C): %f", (float)data.temperature);
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(50));
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

sem_bin = xSemaphoreCreateBinary();
sem_bin2 = xSemaphoreCreateBinary();
mutex = xSemaphoreCreateMutex();

xTaskCreate(vtaskBT, "TASK BT", 2048, NULL, 4, NULL);
xTaskCreate(vtaskRD, "TASK RD", 2048, NULL, 3, NULL);
xTaskCreate(vtaskDP, "TASK DP", 2048, NULL, 2, NULL);

}
