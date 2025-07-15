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
#include "stm32g4xx_hal.h"

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
#define ICAP_Pin GPIO_PIN_0
#define ICAP_GPIO_Port GPIOA
#define VCAP_Pin GPIO_PIN_1
#define VCAP_GPIO_Port GPIOA
#define VIN_Pin GPIO_PIN_3
#define VIN_GPIO_Port GPIOA
#define I_IN_Pin GPIO_PIN_4
#define I_IN_GPIO_Port GPIOA
#define I_OUT_Pin GPIO_PIN_0
#define I_OUT_GPIO_Port GPIOB
#define V_HI_Pin GPIO_PIN_12
#define V_HI_GPIO_Port GPIOB
#define V_LO_Pin GPIO_PIN_14
#define V_LO_GPIO_Port GPIOB
#define HB_PWM_Pin GPIO_PIN_8
#define HB_PWM_GPIO_Port GPIOA
#define HB_EN_Pin GPIO_PIN_9
#define HB_EN_GPIO_Port GPIOA
#define CAPEN_Pin GPIO_PIN_4
#define CAPEN_GPIO_Port GPIOB
#define OUTEN_Pin GPIO_PIN_5
#define OUTEN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define ID_MASTER_CMD 0x210
struct MasterCmd {
	uint8_t maxPowerH;
	uint8_t maxPowerL;
	uint8_t ResetPowerConsump;
	uint8_t RESERVED[5];
};

#define ID_CAPACITY_STATUS 0x211
struct CapacityStatus {
	uint16_t PowerConsump;
	uint16_t Vcap;
	uint16_t TotalEnergy;
	uint16_t CWtarget;
};

struct CanConfig{
		FDCAN_RxHeaderTypeDef rx_header;
		struct MasterCmd rx_msg;
		FDCAN_TxHeaderTypeDef tx_header;
		struct CapacityStatus tx_msg;
		/* Time of last message transfered */
		uint32_t last_can_tx;
		/* Time of last message received */
		uint32_t last_can_rx;
	} ;

extern struct CanConfig can_config;
extern struct MasterCmd master_cmd;
extern struct CapacityStatus cap_status; 
extern struct PID cap_pid;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
