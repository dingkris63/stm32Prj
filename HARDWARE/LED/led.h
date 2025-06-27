#ifndef __LED_H
#define __LED_H
#include "stm32f4xx_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LED0 PFout(9)
#define LED1 PFout(10)

void LED_Init(void);

#ifdef __cplusplus
}
#endif
#endif
