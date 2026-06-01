#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/queue.h>
#include <freertos/timers.h>
#include <freertos/queue.h>
#include <string.h>

#define BT1 GPIO_NUM_4

TimerHandle_t timer1, timer2;
QueueHandle_t fila1, fila2;
TaskHandle_t decode;
uint32_t cnt = 0;

char characters[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'
};
char *morsecode[] = {
    ".-   ", "-...  ", "-.-. ", "-..  ", ".    ", "..-. ", "--.  ", ".... ", "..   ", ".--- ",
    "-.-  ", ".-.. ", "--   ", "-.   ", "---  ", ".--. ", "--.- ", ".-.  ", "...  ", "-    ",
    "..-  ", "...- ", ".--  ","-..- ", "-.-- ", "--.. ",
    ".----", "..---", "...--", "....-", ".....",
    "-....", "--...", "---..", "----.", "-----"
};

static void IRAM_ATTR gpio_ISR(void *args)
{
    BaseType_t hptw;
    hptw = pdFALSE;

    xTimerStartFromISR(timer1, &hptw);

    if(hptw == pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

void timer_char(TimerHandle_t xtimer)
{
   char v;
   if(gpio_get_level(BT1) == 1)
    {
        ESP_LOGI("T", ".");
        v = '.';
        xTimerStart(timer2, 0);
    }
   if(gpio_get_level(BT1) == 0)
    {
        ESP_LOGI("T", "-");
        v = '-';
        xTimerStart(timer2, 0);
    }
    xTimerStop(timer1, 0);
    xQueueSendToBack(fila1, &v, 0);
    
}

void timer_code(TimerHandle_t xtimer)
{
    xTaskNotifyGive( decode );
}

void decodificador(void *pvParameters)
{
    char v;
    char code[5];
    ESP_LOGI("DEC", "%s", code);
    while(1)
    {   
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        for(int i=0; i <= 5; i++)
        { 
          if(xQueueReceive(fila1, &v, 0) == pdTRUE)
          {
           code[i] = v;
          }
          else
          {
            code[i] = ' ';
          }
          
        }   
        xQueueSendToBack(fila2, &code, 0);
        ESP_LOGI("DEC", "%s", code);

    }
}

void tradutor(void *pvParameters)
{
    char code[5];
    while(1)
    {
        xQueueReceive(fila2, &code, portMAX_DELAY);

        for(int i=0; i <=sizeof(characters); i++)
        {
            if(strcmp(morsecode[i],code) == 0)
            {
                ESP_LOGI("Trad", "%c", characters[i]);
            }
        }
    }
}

void app_main(void)
{
    vTaskPrioritySet(NULL, 5);

    // Configurando os pinos:
    gpio_reset_pin(BT1);
    gpio_set_direction(BT1, GPIO_MODE_DEF_INPUT);
    gpio_set_intr_type(BT1, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(BT1);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BT1, gpio_ISR, NULL);

    // Criação do timer
    timer1 = xTimerCreate("Timer_led", pdMS_TO_TICKS(250), pdTRUE, NULL, timer_char);
    timer2 = xTimerCreate("Timer_code", pdMS_TO_TICKS(2000), pdFALSE, NULL, timer_code);

    // Criação da fila
    fila1 = xQueueCreate(5, sizeof(char));
    fila2 = xQueueCreate(1, sizeof(char)*5);

    // Criação das tasks
    xTaskCreate(decodificador, "Decoder", 4098, NULL, 2, &decode);
    xTaskCreate(tradutor, "Traslate", 4098, NULL, 2, NULL);

}
