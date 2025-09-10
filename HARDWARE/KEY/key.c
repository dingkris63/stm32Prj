#include "key.h"
#include "stm32f4xx.h"

void key_init() {
  GPIO_InitTypeDef key_gpio;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  key_gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
  key_gpio.GPIO_Mode = GPIO_Mode_IN;
  // key_gpio.GPIO_OType = ; // No need to set for input
  key_gpio.GPIO_PuPd = GPIO_PuPd_UP;
  key_gpio.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE, &key_gpio);

  key_gpio.GPIO_Pin = GPIO_Pin_0;
  key_gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &key_gpio);
}

key_type get_key() {
  key_type key = 0;
  if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)) {
    key |= HWM_KEY0;
  }
  if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)) {
    key |= HWM_KEY1;
  }
  if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)) {
    key |= HWM_KEY2;
  }
  if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0) {
    key |= HWM_KEY3;
  }

  return key;
}
