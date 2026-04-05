/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define Radiacao_solar_Pin GPIO_PIN_0
#define Radiacao_solar_GPIO_Port GPIOA
#define Nivel_do_rio_Pin GPIO_PIN_1
#define Nivel_do_rio_GPIO_Port GPIOA
#define Velocidade_do_vento_Pin GPIO_PIN_2
#define Velocidade_do_vento_GPIO_Port GPIOA
#define Linha_1_Pin GPIO_PIN_4
#define Linha_1_GPIO_Port GPIOA
#define Linha_2_Pin GPIO_PIN_5
#define Linha_2_GPIO_Port GPIOA
#define Linha_3_Pin GPIO_PIN_6
#define Linha_3_GPIO_Port GPIOA
#define Linha_4_Pin GPIO_PIN_7
#define Linha_4_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_11
#define Buzzer_GPIO_Port GPIOB
#define Barramento_de_dados_Pin GPIO_PIN_12
#define Barramento_de_dados_GPIO_Port GPIOB
#define Barramento_de_dadosB13_Pin GPIO_PIN_13
#define Barramento_de_dadosB13_GPIO_Port GPIOB
#define Barramento_de_dadosB14_Pin GPIO_PIN_14
#define Barramento_de_dadosB14_GPIO_Port GPIOB
#define Barramento_de_dadosB15_Pin GPIO_PIN_15
#define Barramento_de_dadosB15_GPIO_Port GPIOB
#define E_Pin GPIO_PIN_8
#define E_GPIO_Port GPIOA
#define RS_Pin GPIO_PIN_9
#define RS_GPIO_Port GPIOA
#define LED_AMARELO_Pin GPIO_PIN_12
#define LED_AMARELO_GPIO_Port GPIOA
#define LED_VERMELHO_Pin GPIO_PIN_5
#define LED_VERMELHO_GPIO_Port GPIOB
#define Coluna_1_Pin GPIO_PIN_7
#define Coluna_1_GPIO_Port GPIOB
#define Coluna_2_Pin GPIO_PIN_8
#define Coluna_2_GPIO_Port GPIOB
#define Coluna_3_Pin GPIO_PIN_9
#define Coluna_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
