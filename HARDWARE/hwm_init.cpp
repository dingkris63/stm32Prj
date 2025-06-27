#include "FreeRTOS.h"
#include "hwm_pub.h"
#include "led.h"
#include "task.h"
#include <cstdint> // C++ version of stdint.h

// C++ class implementation can go here
class HardwareManager {
  TaskHandle_t myTask;

  static void hwm_main(void *pvParameters) {
    // Cast back to instance if needed
    // HardwareManager* instance = static_cast<HardwareManager*>(pvParameters);
    while (true) {
      GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }

public:
  void init() {
    LED_Init();
    GPIO_SetBits(GPIOF, GPIO_Pin_9);
    GPIO_ResetBits(GPIOF, GPIO_Pin_10);
    xTaskCreate(hwm_main, "Hwm main", configMINIMAL_STACK_SIZE,
                nullptr, // or 'this' if using instance method
                4, &myTask);
    vTaskStartScheduler();
  }
};

// C-compatible wrapper function
void hwm_init(void) {
  static HardwareManager hwm; // or create dynamically
  hwm.init();
}