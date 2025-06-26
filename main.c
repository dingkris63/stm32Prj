#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
static TaskHandle_t myTask;
static TaskHandle_t myTask1;
void BlinkTask9(void *pv) {
    while(1) {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void BlinkTask_10(void *pv) {
    while(1) {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    while(1) {}
}

TaskHandle_t myTaks;
int main(void) {
    LED_Init();
    GPIO_SetBits(GPIOF, GPIO_Pin_10);
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);

    xTaskCreate(BlinkTask9, "Blink", configMINIMAL_STACK_SIZE, NULL, 4, &myTask);
    xTaskCreate(BlinkTask_10, "Blink1", configMINIMAL_STACK_SIZE, NULL, 4, &myTask1);
    vTaskStartScheduler();

    while(1) {}
}
