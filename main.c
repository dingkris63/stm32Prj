#include "FreeRTOS.h"
#include "task.h"
#include "hwm_pub.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    while(1) {}
}

TaskHandle_t myTaks;

int main() {
    hwm_init();
    while(1);

    return 0;
}