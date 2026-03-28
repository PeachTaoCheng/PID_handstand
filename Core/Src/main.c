/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "RP.h"
#include "pid.h"
#include "key.h"
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
pid_struct angle_pid = 
{
  .kp = 0.310,
  .ki = 0.014,
  // .kd = 0.504,
  .kd = 0.40,

  .target = angle_target,         

  .output_max = 100,
  .output_min = -100,
};

pid_struct location_pid = 
{
  .kp = 0,
  .ki = 0.0,
  .kd = 0,

  .target = 0,

  .output_max = 60,
  .output_min = -60,
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t System_timer;
volatile uint32_t System_timer_angle;
volatile uint32_t System_timer_location;

volatile uint8_t  pid_flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  OLED_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc2);
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    location_pid.kp = (float)((RP_GetValue(2)/4095.0f)*2.0f);
    location_pid.target = (int16_t)((float)((RP_GetValue(3)/4095.0f)*816.0f - 408.0f));
    location_pid.kd = (float)((RP_GetValue(4)/4095.0f)*10.0f);

    OLED_Printf(0, 0, OLED_6X8, "Location");
    OLED_Printf(64, 0, OLED_6X8, "Angle");

    OLED_ShowFloatNum(0, 8, location_pid.kp, 1, 3, OLED_6X8);
    OLED_ShowFloatNum(0, 16, location_pid.ki, 1, 3, OLED_6X8);
    OLED_ShowFloatNum(0, 24, location_pid.kd, 1, 3, OLED_6X8);
    OLED_Printf(0, 32, OLED_6X8, "tar:%+05d", (int16_t)location_pid.target);
    OLED_Printf(0, 40, OLED_6X8, "act:%+05d", (int16_t)location_pid.actual);
    OLED_Printf(0, 48, OLED_6X8, "out:%+05d", (int16_t)location_pid.output);
     OLED_Printf(0, 56, OLED_6X8, "err:%+05d", (int16_t)location_pid.error);

    OLED_ShowFloatNum(64, 8, angle_pid.kp, 1, 3, OLED_6X8);
    OLED_ShowFloatNum(64, 16, angle_pid.ki, 1, 3, OLED_6X8);
    OLED_ShowFloatNum(64, 24, angle_pid.kd, 1, 3, OLED_6X8);
    OLED_Printf(64, 32, OLED_6X8, "tar:%+05d", (int16_t)angle_pid.target);
    OLED_Printf(64, 40, OLED_6X8, "act:%+05d", (int16_t)angle_pid.actual);
    OLED_Printf(64, 48, OLED_6X8, "out:%+05d", (int16_t)angle_pid.output);
    OLED_Update();

    if((angle_pid.actual > angle_target - 500) && (angle_pid.actual < angle_target + 500))
    {
      if(key_state() == 1)
      {
        pid_flag = 1;
        angle_pid.integral = 0;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);    //led点亮，提示进入目标角度且可以进行PID
      }
    }
    else 
    {
      pid_flag = 0;
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    }

    // if((location_pid.error <= 10) && (location_pid.error >= -10))
    // {
    //   location_pid.ki = 0.001f;
    // }
    // else 
    // {
    //   location_pid.ki = 0.000f;
    // }
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
