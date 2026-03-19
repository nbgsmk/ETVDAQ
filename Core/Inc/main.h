/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C_SENS hi2c1
#define SPI_main hspi2
#define UART_radio huart1
#define UART_dbg huart2
#define BOARD_LED0_Pin GPIO_PIN_13
#define BOARD_LED0_GPIO_Port GPIOC
#define KBDsel_L_Pin GPIO_PIN_0
#define KBDsel_L_GPIO_Port GPIOA
#define KBDsel_M_Pin GPIO_PIN_1
#define KBDsel_M_GPIO_Port GPIOA
#define KBDsel_H_Pin GPIO_PIN_3
#define KBDsel_H_GPIO_Port GPIOA
#define Exti_d1_Pin GPIO_PIN_4
#define Exti_d1_GPIO_Port GPIOA
#define Exti_d1_EXTI_IRQn EXTI4_IRQn
#define KBD2_Pin GPIO_PIN_12
#define KBD2_GPIO_Port GPIOB
#define KBD3_Pin GPIO_PIN_13
#define KBD3_GPIO_Port GPIOB
#define Exti_d2_Pin GPIO_PIN_8
#define Exti_d2_GPIO_Port GPIOA
#define Exti_d3_Pin GPIO_PIN_15
#define Exti_d3_GPIO_Port GPIOA
#define KBD0_Pin GPIO_PIN_4
#define KBD0_GPIO_Port GPIOB
#define KBD1_Pin GPIO_PIN_5
#define KBD1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
