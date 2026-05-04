#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/queue.h>
#include "dht11.h"

#define AMOSTRAS 10

struct media_movel
{
    float temperatura;
    float humidade;
};

struct db
{
    float temperatura[AMOSTRAS];
    float humidade[AMOSTRAS]; 
};

QueueHandle_t queue, med_movel;
SemaphoreHandle_t sem_bin1, sem_bin2, sem_bin3;
struct media_movel media;
bool flag = 1;


void Coleta(void *pvparameters)
{
    ESP_LOGI("Coleta", "task inicializando");
    struct dht11_reading data1;
    struct db data;
    int i;
    while (1)
    {
        if(flag == 1)
        {    
            for(i = 0; i < AMOSTRAS ;i++)
            {
                data1 = DHT11_read();
                data.temperatura[i] = data1.temperature;
                data.humidade[i] = data1.temperature;
            }
            xQueueSendToBack(queue, &data, portMAX_DELAY);
            xSemaphoreGive(sem_bin1);
            flag = 0;
        }
       vTaskDelay(pdMS_TO_TICKS(50));    
    }
    
}


void Media_Temp(void *pvparameters)
{
    ESP_LOGI("Media Temperatura", "task inicializando");
    struct dht11_reading data;
    float temp[AMOSTRAS];
    int i;
    while(1)
    {
        xSemaphoreTake(sem_bin1, portMAX_DELAY);
        xQueueReceive(queue, &data, portMAX_DELAY);

            for(i = 0; i< 10; i++)
            {
                if(i == 0)
                {
                    media.temperatura += (1/AMOSTRAS)*(temp[i]); 
                }
                else
                {
                    media.temperatura += (1/AMOSTRAS)*(temp[i] - temp[i-1]); 
                }
                
            }  
        xSemaphoreGive(sem_bin2);
        xSemaphoreGive(sem_bin1);
    }
}

void Media_Humid(void *pvparameters)
{
    ESP_LOGI("Media Temperatura", "task inicializando");
    int cnt = 0;
    float humid[AMOSTRAS];
    struct dht11_reading data;
    while(1)
    {
        xSemaphoreTake(sem_bin2, portMAX_DELAY);
        xQueueReceive(queue, &data, portMAX_DELAY);
        if(cnt < 10)
        {
            humid[cnt] = data.humidity;
            cnt++;
        }
        else
        {
            int i;
            
            for(i = 0; i< 10; i++)
            {
                if(i == 0)
                {
                    media.humidade += (1/AMOSTRAS)*(humid[i]); 
                }
                else
                {
                    media.humidade += (1/AMOSTRAS)*(humid[i] - temp[i-1]); 
                }
                
            }
            cnt = 0;
            xQueueSendToBack(med_movel, &media, portMAX_DELAY);
            xSemaphoreGive(sem_bin3);
        }

    }
    
}

void Display(void *pvparameters)
{
    ESP_LOGI("Display", "task inicializando");
    while(1)
    {
        xSemaphoreTake(sem_bin3, portMAX_DELAY);
        xQueueReceive(med_movel, &media, portMAX_DELAY);
        ESP_LOGW("DP", "Média Móvel da Temperatura (°C) %f", (float)media.temperatura);
        ESP_LOGW("DP", "Média Móvel da Temperatura (%%) %f", (float)media.humidade);
        xSemaphoreGive(sem_bin3);
    }
        
}

void app_main(void)
{
    vTaskPrioritySet(NULL, 5);
    gpio_set_pull_mode(DHT11_PIN, GPIO_PULLUP_ONLY);
    DHT11_init(DHT11_PIN);

    xSemaphoreCreateBinary(sem_bin1);
    xSemaphoreCreateBinary(sem_bin2);
    xSemaphoreCreateBinary(sem_bin3);

    xTaskCreate(vtaskColeta, "TASK Coleta", 2048, NULL, 4, NULL);
    xTaskCreate(vtaskMedia, "TASK Media", 2048, NULL, 3, NULL);
    xTaskCreate(vtaskMedia, "TASK Display", 2048, NULL, 3, NULL);
}
