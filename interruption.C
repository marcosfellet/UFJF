#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "ultrasonic.h"


#define MAX_DISTANCE_CM 500
#define TRIGGER_GPIO 18
#define ECHO_GPIO 5
#define LIMIAR_M 0.1
#define TAM_FILA 20


#define BTIO 16
#define LDIO 17

// struct do sensor

ultrasonic_sensor_t sensor;

SemaphoreHandle_t sem_bin;
QueueHandle_t fila;

float limiar = LIMIAR_M;

static void IRAM_ATTR gpio_ISR(void *args){
    BaseType_t hptw;
    hptw = pdFALSE;
    if(gpio_get_level(BTIO) == 0)
    {
        limiar += 0.1;
        if(limiar > 5)
        {
            limiar = 5;
        }
        //xSemaphoreGiveFromISR(sem_bin, &hptw);
        hptw = pdTRUE;
    }
    if(gpio_get_level(LDIO) == 0)
    {
        limiar -= 0.1;
        if(limiar == 0)
        {
            limiar = 0.1;
        }
        //xSemaphoreGiveFromISR(sem_bin, &hptw);
        hptw = pdTRUE;
    }

    if(hptw == pdTRUE){
        portYIELD_FROM_ISR();
    }
}


void leitura(void *pvparameters)
{
    ESP_LOGI("Leitura", "Task Inicializada");
    float dist;
    while(1)
    {
        //xSemaphoreTake(sem_bin, portMAX_DELAY);
        ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &dist);
        xQueueSendToBack(fila, &dist, portMAX_DELAY);
        ESP_LOGI("DP", "Valor medido: %.3f", dist);
        vTaskDelay(pdMS_TO_TICKS(10));
        
    }

}

void alarme(void *pvparameters)
{
    ESP_LOGI("Alarme", "Task Inicializada");
    float distance;
    while(1)
    {
        xQueueReceive(fila, &distance, portMAX_DELAY);
        if(distance <= limiar)
        {
            ESP_LOGW("Alerta", "Atividades Semafóricas Excessivas");
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
    // Configuração do hardware

    gpio_reset_pin(LDIO);
    gpio_set_direction(LDIO, GPIO_MODE_DEF_INPUT);

    gpio_reset_pin(BTIO);
    gpio_set_direction(BTIO, GPIO_MODE_DEF_INPUT);


    gpio_set_intr_type(BTIO, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(BTIO);
    gpio_set_intr_type(LDIO, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(LDIO);


    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTIO, gpio_ISR, NULL);
    gpio_isr_handler_add(LDIO, gpio_ISR, NULL);

    // Criar semáforo
    
    sem_bin = xSemaphoreCreateBinary();
    
    // Criar a fila

    fila = xQueueCreate(TAM_FILA, sizeof(float));

    // pinouts
    sensor.trigger_pin = TRIGGER_GPIO;
    sensor.echo_pin = ECHO_GPIO;
    
    // inicializa a função do sensor
    ultrasonic_init(&sensor);

    // Criar task

    xTaskCreate(alarme, "Alarme", 2048, NULL, 2, NULL);
    xTaskCreate(leitura, "Leitura", 2048, NULL, 3, NULL);


}
