#include "FreeRTOS.h"
#include "hwm_pub.h"
#include "task.h"
#include <stdio.h>

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
  while (1) {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
  printf("Assert failed in %s, line %lu\n", file, line);
  while (1) {
    // Halt or reboot here
  }
}
#endif

TaskHandle_t myTaks;

int main() {
  hwm_init();
  while (1) {
  }

  return 0;
}