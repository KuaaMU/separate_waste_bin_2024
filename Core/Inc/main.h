/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#define Infrared_switch1_Pin GPIO_PIN_0
#define Infrared_switch1_GPIO_Port GPIOF
#define Infrared_switch1_EXTI_IRQn EXTI0_IRQn
#define Reflect_switch1_Pin GPIO_PIN_1
#define Reflect_switch1_GPIO_Port GPIOF
#define Reflect_switch1_EXTI_IRQn EXTI1_IRQn
#define Reflect_switch2_Pin GPIO_PIN_2
#define Reflect_switch2_GPIO_Port GPIOF
#define Reflect_switch2_EXTI_IRQn EXTI2_IRQn
#define Reflect_switch3_Pin GPIO_PIN_3
#define Reflect_switch3_GPIO_Port GPIOF
#define Reflect_switch3_EXTI_IRQn EXTI3_IRQn
#define Reflect_switch4_Pin GPIO_PIN_4
#define Reflect_switch4_GPIO_Port GPIOF
#define Reflect_switch4_EXTI_IRQn EXTI4_IRQn
#define sonar3_Echo_Pin GPIO_PIN_2
#define sonar3_Echo_GPIO_Port GPIOA
#define sonar3_Trig_Pin GPIO_PIN_4
#define sonar3_Trig_GPIO_Port GPIOA
#define sonar1_Echo_Pin GPIO_PIN_6
#define sonar1_Echo_GPIO_Port GPIOA
#define sonar1_Trig_Pin GPIO_PIN_7
#define sonar1_Trig_GPIO_Port GPIOA
#define sonar4_Trig_Pin GPIO_PIN_2
#define sonar4_Trig_GPIO_Port GPIOB
#define Motor1_Dir_Pin GPIO_PIN_10
#define Motor1_Dir_GPIO_Port GPIOE
#define Motor2_Dir_Pin GPIO_PIN_12
#define Motor2_Dir_GPIO_Port GPIOE
#define DCmotor1_IN1_Pin GPIO_PIN_14
#define DCmotor1_IN1_GPIO_Port GPIOE
#define Motor3_Dir_Pin GPIO_PIN_15
#define Motor3_Dir_GPIO_Port GPIOE
#define DCmotor1_IN2_Pin GPIO_PIN_10
#define DCmotor1_IN2_GPIO_Port GPIOB
#define soanr4_Trig_Pin GPIO_PIN_13
#define soanr4_Trig_GPIO_Port GPIOB
#define sonar4_Echo_Pin GPIO_PIN_14
#define sonar4_Echo_GPIO_Port GPIOB
#define sonar2_Trig_Pin GPIO_PIN_11
#define sonar2_Trig_GPIO_Port GPIOD
#define sonar2_Echo_Pin GPIO_PIN_12
#define sonar2_Echo_GPIO_Port GPIOD
#define sonar2_TrigD15_Pin GPIO_PIN_15
#define sonar2_TrigD15_GPIO_Port GPIOD
#define Motor5_In1_Pin GPIO_PIN_3
#define Motor5_In1_GPIO_Port GPIOB
#define Motor5_In2_Pin GPIO_PIN_4
#define Motor5_In2_GPIO_Port GPIOB
#define Motor5_In3_Pin GPIO_PIN_5
#define Motor5_In3_GPIO_Port GPIOB
#define Motor5_In4_Pin GPIO_PIN_6
#define Motor5_In4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
