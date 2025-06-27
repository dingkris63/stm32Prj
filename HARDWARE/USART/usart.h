/**
 ******************************************************************************
 * @file    USART/USART_Printf/main.h
 * @author  MCD Application Team
 * @version V1.8.1
 * @date    27-January-2022
 * @brief   Header for main.c module
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 0 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART_Config(void);
#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
