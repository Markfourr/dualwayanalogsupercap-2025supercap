/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensor.h"
#include "adc.h"
#include "gpio.h"
#include "fdcan.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static HAL_StatusTypeDef CAN_STATUS;
static HAL_StatusTypeDef CAN_RECEIVE_STATUS;
CAP_STATE_t cap_state = CAP_INIT;
char if_protect = 0;
uint32_t fdcan_received;
uint32_t energy_accum_time;
uint8_t ResetPowerConsump;
uint32_t EnergyTmp;
int32_t temp_P_Target = 0;
float TotalEnergy_float = 0;
struct BadPWRContext BadPowerContext= {0, 0, 0,0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  HAL_RCC_NMI_IRQHandler();
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */
			//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
	
	/* This should run every two iter. 100KHz, t=75% * T. */
	static uint8_t flag = 0;
	if ((flag = flag ^ 1) == 0)
	{
		/* Handle Injected Registers */
		V_IN  = V_IN  - (V_IN  >> 3) + hadc1.Instance->JDR2;
		V_LO  = V_LO  - (V_LO  >> 3) + hadc2.Instance->JDR2;
		V_CAP = V_CAP - (V_CAP >> 3) + hadc3.Instance->JDR2;
		V_HI  = V_HI  - (V_HI  >> 3) + hadc4.Instance->JDR2;
		
		/* If Half Bridge Output Enabled: */
		if ((HB_EN_GPIO_Port->ODR & HB_EN_Pin) &&
			  (cap_state !=CAP_PROTECT) &&
		    (!PROTECT_CONDITION))
		{
			/* Update pulse width. Note that pulse width is 8 times of actual PWM pulse width. */
			pulse_width_mul8 +=  (VLO_target - V_LO) / 80;
			pulse_width_mul8 = VAL_LIMIT(pulse_width_mul8, 10<<3, 1580<<3);
			

			htim1.Instance->CCR1 = (pulse_width_mul8 >> 3);
			
			/* Set Sampling position to half of pulse */
			htim1.Instance->CCR4 = (pulse_width_mul8 >> 4);
			
		}
	}
	else
	{
		I_CAP = I_CAP - (I_CAP >> 3) + hadc1.Instance->JDR1 - (1<<11);
		I_IN  = I_IN  - (I_IN  >> 3) - hadc2.Instance->JDR1 + (1<<11);
		I_OUT = I_OUT - (I_OUT >> 3) - hadc3.Instance->JDR1 + (1<<11);
		V_REF = V_REF - (V_REF >> 3) + hadc4.Instance->JDR1;
	}
	
		//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
	/* Clear Flag and Return. Let HAL Handler Fuck Off. */
	__HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4|TIM_FLAG_CC5|TIM_FLAG_CC6);
	return;
  /* USER CODE END TIM1_CC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */
	
  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	// This should run at 10000Hz.

	if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0)) {
			can_config.last_can_rx = HAL_GetTick();
			CAN_RECEIVE_STATUS = HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &can_config.rx_header, (uint8_t*)&can_config.rx_msg);
		// Magic Number: 875.35014 == 1 / 0.0011424, 0.01 = 1 / 100
			if (can_config.rx_header.Identifier == ID_MASTER_CMD)
			{
				//875.35014f is P needed and 800 is the value we adjust
				temp_P_Target = (int32_t)((((uint16_t)can_config.rx_msg.maxPowerH << 8) | can_config.rx_msg.maxPowerL) * 0.01f * 902.77);
				// If P_Target received is invalid, then override it to 55000
				// 35000 for 45W, 85000 for 120W
				if (temp_P_Target <= 10000 || temp_P_Target >= 150000)	{
					temp_P_Target = 30000;
				}
				if(V_CAP>16500){
					temp_P_Target = temp_P_Target * 0.93;
				}
				P_Target = temp_P_Target;
				ResetPowerConsump = can_config.rx_msg.ResetPowerConsump;
			}
		}
	

	
	// Convert fixed-point numbers to SI unit
	can_config.last_can_tx = HAL_GetTick();										
	can_config.tx_msg.PowerConsump = (V_HI * 0.0011077f * I_OUT * 0.0010070f) * 10.f ;		
	can_config.tx_msg.TotalEnergy = (uint16_t) TotalEnergy_float;
	can_config.tx_msg.Vcap = (uint32_t)((V_CAP * 0.0011077f) * 100.f);						
	can_config.tx_msg.CWtarget = P_Target * 0.00125f * 100.f;										
	CAN_STATUS = HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &can_config.tx_header, (uint8_t*)&can_config.tx_msg);
	
	/* Clear Flag and Return. Let HAL Handler Fuck Off. */
	__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);
	return;
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC3 channel underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* We will Mannually Handel System Tick Here. */
  HAL_IncTick();
  __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
  return;
  
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt, DAC2 and DAC4 channel underrun error interrupts.
  */
void TIM7_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_DAC_IRQn 0 */
	/* Main Control Logic Here. 10KHz */
	
	/****************************************************************
	** Table For Mapping Between Fix-point Value to Actual Value:  **
	** ----------------------------------------------------------  **
	** |   Voltage   |   1 VLSB   |     1.1077880859375   mV    |  **
	** |   Current   |   1 ALSB   |     1.0070800781250   mA    |  **
	** |    Power*   |   1 WLSB   |     1.1156313121319   uW    |  **
	** |    Power    |  1 KiWLSB  |     1.1424064636230   mW    |  **
	** ----------------------------------------------------------  **
	**               All variables are in int32_t.                 **
	**       For Voltage, the value should be in 0 ~ 2^15-1.       **
	**     For Current, the value should be in -2^14 ~ 2^14-1.     **
	**       For Power, the value is moved 10 bit right,           **
	**           which is in (-2^18 ~ 2^18-1) * 2^10.              **
	**********************!z******************************************/
	
	// Increse protection_cnt if we're in CAP_PROTECT
	// We cannot leave CAP_PROTECT until protection_cnt reached 15000 (1.5s)
	static uint32_t protection_cnt = 0;
	
	P_real = (V_IN * I_IN) / 1000;
	P_cap_real = (V_CAP * I_CAP)/1000;
	
	if(if_protect && PROTECT_CONDITION){
		/*Firstly turn off the HB*/	
		HAL_GPIO_WritePin(CAPEN_GPIO_Port, CAPEN_Pin, GPIO_PIN_RESET);
		cap_state = CAP_PROTECT;
	}
	
	int32_t VLO_Lbound = V_CAP_MIN;
	
	switch (cap_state)
	{
		case CAP_INIT:
		{
			/* Init state */
			HAL_GPIO_WritePin(HB_EN_GPIO_Port, HB_EN_Pin, GPIO_PIN_SET);
			/* Target: V_LO == V_CAP */
			VLO_target = V_CAP;
			int32_t err = VLO_target - V_LO;
			VLO_Lbound = 100;
			if (ABS(err) < 1000 && !(PROTECT_CONDITION))
			{
				cap_state = CAP_PIN;
				if_protect = 1;
			}		
			break;
		}	
		case CAP_ICAP_IN:
		{
			/* Limit Cap Current + */
			int32_t err = I_CAP_MAX - I_CAP;
			VLO_target += err >> 5;
			if (I_CAP < I_CAP_MAX_LO)
			{
				cap_state = CAP_PIN;
			}

			break;
		}
		case CAP_ICAP_OUT:
		{
			/* Limit Cap Current - */
			int32_t err = I_CAP_MAX + I_CAP;
			VLO_target -= err >> 5;
			if (-I_CAP < I_CAP_MAX_LO)
			{
				cap_state = CAP_PIN;
			}

			break;
		}
		
		case CAP_VCAP:
		{
			/* Limit Cap Voltage - */
			VLO_Lbound = V_CAP_MIN + 1000;
			
			int32_t err = 1;
			VLO_target += err >> 3;	
			
			if (V_CAP > (V_CAP_MIN + 1000))
			{
				HAL_GPIO_WritePin(OUTEN_GPIO_Port, OUTEN_Pin, GPIO_PIN_SET);
				cap_state = CAP_PIN;
			}		
			
			/* Intention Remove BREAK to run CAP PIN processing. */
			break;
		}
		
		case CAP_PIN:
		{
			int32_t err = P_Target - P_real ;			
			HAL_GPIO_WritePin(CAPEN_GPIO_Port, CAPEN_Pin, GPIO_PIN_SET);
			
			/* Const Input Power */
			/* Target: P_IN == P_Target */
			//VLO_target += err >> 8;
			
			if (V_CAP < 16000){
				VLO_target += err >> 8;
			}
			else {
				VLO_target += err >> 9;
			}

			//if (+I_CAP > I_CAP_MAX_HI)
			//{
				//cap_state = CAP_ICAP_IN;
			//}
			//if (-I_CAP > I_CAP_MAX_HI)
			//{
				//cap_state = CAP_ICAP_OUT;
			//}
			if (V_CAP < V_CAP_MIN)
			{
				cap_state = CAP_VCAP;
				VLO_target = V_CAP;
			}
			break;
		}
		
		case CAP_PROTECT:
		{
			/*Protect Mode*/
			CAPEN_GPIO_Port->BRR = (uint32_t)CAPEN_Pin;
			VLO_Lbound = V_CAP_MIN;
			VLO_target = V_CAP;
			
			if (!PROTECT_CONDITION)
			{
				protection_cnt++;
				// if voltage is stable within 0.1s
				if (protection_cnt > PROTECT_EXIT_TIME)
				{
					protection_cnt = 0;
					cap_state = CAP_INIT;
				}
			}
			else
			{protection_cnt = 0;}
			HAL_GPIO_WritePin(HB_EN_GPIO_Port, HB_EN_Pin, GPIO_PIN_RESET);
			
				/*Can't change to other mode.*/
				/*  */
		break;
		}
		
		case CAP_I_WORK_PROTECT:
			{
			CAPEN_GPIO_Port->BRR = (uint32_t)CAPEN_Pin;
		}
		
		default:
		{
			;
			}
		}
	
			//some ieals to fuck off chassis and fuck off the cap
		//if (P_real > P_Target) { 
		//	BadPowerContext.cnt_VOUT_OVERPOWER =BadPowerContext.cnt_VOUT_OVERPOWER  +1;
		//	BadPowerContext.cnt_VOUT_OVERPOWER = 0;
		//	if (BadPowerContext.cnt_VOUT_OVERPOWER > 50){
		//			cap_state = CAP_VCAP;
		//			HAL_GPIO_WritePin(OUTEN_GPIO_Port, OUTEN_Pin, GPIO_PIN_RESET);
		//			BadPowerContext.cnt_VOUT_OVERPOWER = 0;
	//		}
	//	}
	//	else {
	//		BadPowerContext.cnt_VOUT_OVERPOWER = 0;}
			
		if (-I_IN > 200) { 
			BadPowerContext.cnt_VIN_REVERSE_CURRENT = BadPowerContext.cnt_VIN_REVERSE_CURRENT + 1;
			//1000 is 0.1s
			if (BadPowerContext.cnt_VIN_REVERSE_CURRENT > 1000){
				cap_state = CAP_PROTECT;
			  BadPowerContext.cnt_VIN_REVERSE_CURRENT = 0;
		}
		} 
		else {
			BadPowerContext.cnt_VIN_REVERSE_CURRENT = 0;}
		
		//if ((-P_cap_real > 3500)&&(-P_cap_real < 13000)&&(P_real < 8000)) { 
			if ((-P_cap_real > P_real)&&(P_real < 8000)) { 
			BadPowerContext.cnt_VIN_STAY_CURRENT = BadPowerContext.cnt_VIN_STAY_CURRENT + 1;
			//5000 is 0.5s
			if (BadPowerContext.cnt_VIN_STAY_CURRENT > 5000){
				cap_state = CAP_PROTECT;
			  BadPowerContext.cnt_VIN_STAY_CURRENT = 0;
		}
		} 
		else {
			BadPowerContext.cnt_VIN_STAY_CURRENT = 0;}
			
		if ((I_CAP > I_CAP_MAX_HI)||(-I_CAP > I_CAP_MAX_HI)) { 
			BadPowerContext.cnt_I_CAP_PROTECT = BadPowerContext.cnt_I_CAP_PROTECT + 1;
			//500 is 0.5s
			if (BadPowerContext.cnt_I_CAP_PROTECT > 500){
				cap_state = CAP_PROTECT;
			  BadPowerContext.cnt_I_CAP_PROTECT = 0;
		}
		} 
		else {
			BadPowerContext.cnt_I_CAP_PROTECT = 0;}
		
	
		
		/* Limit VCAP Target in VLO_Lbound ~ V_CAP_MAX */
		VLO_target = VAL_LIMIT(VLO_target, VLO_Lbound, V_CAP_MAX);
		
		// Calculate Power Consump
		// We accumulate energy every 10000 timer ticks(1 second).
		// The unit of energy is mW ((V * I) &gt;&gt; 10) * (1 / 10000) s == 0.1 uJ or 100nJ.
		energy_1s_in_100nJ += P_real;
		//EnergyTmp += P_real;
		if((energy_accum_time++ % TIM7_FREQ))
		{
			if (ResetPowerConsump)
			{
				ResetPowerConsump = 0;
				cap_status.PowerConsump = 0;
			}
			// Convert unit from 100nJ to 1J, and do fixed-point conversion
			// Magic Number: 1 Ki J LSB = 1.1424064636230 mJ
			TotalEnergy_float += energy_1s_in_100nJ * 0.0000001f * 1.1424064636230f;
			energy_1s_in_100nJ = 0;
			energy_accum_time = 0;
		}
		
	/* Clear Flag and Return. Let HAL Handler Fuck Off. */
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	return;
  /* USER CODE END TIM7_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_DAC_IRQn 1 */

  /* USER CODE END TIM7_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
