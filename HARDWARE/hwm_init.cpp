#include "FreeRTOS.h"
#include "hwm_pub.h"
#include "key.h"
#include "led.h"
#include "stm32f4xx.h"
#include "task.h"
#include "usart.h"
#include "queue.h"
#include <cstdint> // C++ version of stdint.h

static TaskHandle_t ledTaskHandle;

// C++ class implementation can go here
class HardwareManager {

  static void led_main(void *pvParameters) {
    LED_Init();
    GPIO_SetBits(GPIOF, GPIO_Pin_9);
    GPIO_ResetBits(GPIOF, GPIO_Pin_10);
    while (true) {
      GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }

  static void key_main(void *pvParameters) {
    key_init();
    vTaskDelete(ledTaskHandle);
    LED_Init();
    GPIO_SetBits(GPIOF, GPIO_Pin_9);
    GPIO_ResetBits(GPIOF, GPIO_Pin_10);
    uint32_t count = 0;
    while (1) {
      auto key = get_key();
      if(key & HWM_KEY0) {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
      }
      if(key & HWM_KEY1) {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
      }
      if(key & HWM_KEY2) {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
      }
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }

public:
  void init() {

    xTaskCreate(led_main, "Led main", configMINIMAL_STACK_SIZE, nullptr, 4,
                &ledTaskHandle);
    xTaskCreate(key_main, "Key main", configMINIMAL_STACK_SIZE, nullptr, 4,
                NULL);
    vTaskStartScheduler();
  }
};

// C-compatible wrapper function
void hwm_init(void) {
  static HardwareManager hwm; // or create dynamically
  hwm.init();
}
