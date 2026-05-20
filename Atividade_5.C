#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/queue.h>
#include "dht11.h"

// defines

#define LIMITE_TEMP 100
#define LIMITE_HUMID 500 
#define TAM_MENSAGEM 40
#define QUANTIDADE_MENSAGENS_RECEBER 1
#define JANELA_MEDIA 10
#define BIT0 1<<0
#define BIT1 1<<1
#define DHT11_PIN 4


// definicao de uma struct para colocar uma id 

// Declaração da Variaveis Globais:

EventGroupHandle_t event_group;

StreamBufferHandle_t buffer_temp, buffer_humid, buffer_media_temp, bufffer_media_humid;

TaskHandle_t alarme_handle, coleta_handle;  

// Tasks

void coleta(void *pvparameters)
{
    ESP_LOGI("Start", "Inicializando a Coleta");
    struct dht11_reading data;
    while(1)
    {
        data = DHT11_read();
        if(data.temperature > LIMITE_TEMP || data.humidity > LIMITE_HUMID)
        {
            xTaskNotifyGive(alarme_handle);
        }
        xStreamBufferSend(buffer_temp, &data.temperature, sizeof(data.temperature), portMAX_DELAY);
        xStreamBufferSend(buffer_humid, &data.humidity, sizeof(data.humidity), portMAX_DELAY);
    }
}

void media_movel_temp(void *pvparameters)
{
    ESP_LOGI("Start", "Calculando a Media Movel");
    struct dht11_reading data;
    float media_movel_temp = 0;
    char i;
    while(1)
    {
        for(i = 0; i < JANELA_MEDIA; i++) 
        {
            // Recebendo Dados
            data.temperature = xStreamBufferReceive(buffer_temp, &data.temperature, sizeof(data.temperature), portMAX_DELAY);
            // Calcula as médias e 
            media_movel_temp = (1/JANELA_MEDIA)*(media_movel_temp+data.temperature);
        }
        xEventGroupSetBits(event_group, BIT0);
        xStreamBufferSend(buffer_media_temp, &media_movel_temp, sizeof(media_movel_temp), portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void media_movel_humid(void *pvparameters)
{
    ESP_LOGI("Start", "Calculando a Media Movel");
    struct dht11_reading data;
    float media_movel_humid = 0;
    char i;
    while(1)
    {
        for(i = 0; i < JANELA_MEDIA; i++) 
        {
            // Recebendo Dados
            data.humidity = xStreamBufferReceive(buffer_humid, &data.humidity, sizeof(data.humidity), portMAX_DELAY);
            // Calcula a média 
            media_movel_humid = (1/JANELA_MEDIA)*(media_movel_humid+data.humidity);
        }
        xEventGroupSetBits(event_group, BIT1);
        xStreamBufferSend(bufffer_media_humid, &media_movel_humid, sizeof(media_movel_humid), portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void display(void *pvparameters)
{
    ESP_LOGI("Start", "Inicializando o Display");
    float media_temp, media_humid = 0;
    while(1)
    {
        xEventGroupWaitBits(event_group, BIT0|BIT1, pdTRUE, pdTRUE, portMAX_DELAY);  
        media_temp = xStreamBufferReceive(buffer_media_temp, &media_temp, sizeof(media_temp), portMAX_DELAY);
        media_humid = xStreamBufferReceive(bufffer_media_humid, &media_humid, sizeof(media_humid), portMAX_DELAY);
        ESP_LOGI("Valor calculado!","Media da Temperatura (°C) = %.3f", media_temp);
        ESP_LOGI("Valor calculado!","Media da Humidade (%%) = %.3f", media_humid);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void alarme(void *pvparameters)
{
    ESP_LOGI("Start", "Inicializando o Alarme");
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGE("ERROR", "Valores de temperatura ou humidade acima do limite");
    }
}

void app_main(void)
{

    // Configurando os pinos:
    gpio_set_pull_mode(DHT11_PIN, GPIO_PULLUP_ONLY);
    DHT11_init(DHT11_PIN);

    // Criando Buffer
    buffer_temp = xStreamBufferCreate(TAM_MENSAGEM, QUANTIDADE_MENSAGENS_RECEBER);
    buffer_humid = xStreamBufferCreate(TAM_MENSAGEM, QUANTIDADE_MENSAGENS_RECEBER);
    buffer_media_temp = xStreamBufferCreate(TAM_MENSAGEM, QUANTIDADE_MENSAGENS_RECEBER);
    bufffer_media_humid = xStreamBufferCreate(TAM_MENSAGEM, QUANTIDADE_MENSAGENS_RECEBER);

    // Criando o Group Event
    event_group = xEventGroupCreate();

    // Criando as tasks
    xTaskCreate(coleta, "coleta", 2048, NULL, 2, &coleta_handle);
    xTaskCreate(media_movel_temp, "media_movel_temp", 2048, NULL, 2, NULL);
    xTaskCreate(media_movel_humid, "media_movel_humid", 2048, NULL, 2, NULL);
    xTaskCreate(display, "display", 2048, NULL, 2, NULL);
    xTaskCreate(alarme, "alarme", 2048, NULL, 2, &alarme_handle);
}
