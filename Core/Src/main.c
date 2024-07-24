/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nortos.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define MOISTURE_LOW_TH 2640

uint32_t bufADC[8];
int count = 1900;
char moistureIsLow = 0;


void StopSTP(void){
// LL_TIM_OC_SetCompareCH4(TIM1, 0xFFFF);
}

void readADC(void){

  static unsigned int i  = 0;
  while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0);
  LL_ADC_REG_StartConversion(ADC1);
  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0);
  bufADC[i%8] = LL_ADC_REG_ReadConversionData12(ADC1);
  i++;
  LL_ADC_ClearFlag_EOC(ADC1);
  }


void evaluateMoisture(void){
  int result = 0;
  for(int i = 0; i < 8; i++) 
    result += bufADC[i];
  
  if(result/8 > MOISTURE_LOW_TH)
    moistureIsLow = 1;
  else
    moistureIsLow = 0;

}
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
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, 3);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM14_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  LL_TIM_ClearFlag_UPDATE(TIM14); // nejprve pro jistotu vymažeme vlajku
  LL_TIM_EnableIT_UPDATE(TIM14); 
  LL_TIM_EnableCounter(TIM14);
  
  F1_QueueIni();
  F2_QueueIni();
  
  LL_ADC_EnableInternalRegulator(ADC1);
  LL_mDelay(1);
  LL_ADC_ClearFlag_ADRDY(ADC1);
  LL_ADC_Enable(ADC1);
 

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    F1_pull()();

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_SetHSIDiv(LL_RCC_HSI_DIV_16);
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(1000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(1000000);
}

/* USER CODE BEGIN 4 */



void TIM14_Reload_Callback(void){
 //static int count = 0;
  
  switch(count){
      case 1:
   //            LL_GPIO_SetOutputPin(GPIOB, SENSOR_Pin); // Disable Sensor
               if(moistureIsLow)
                    LL_GPIO_SetOutputPin(GPIOB, WATER_Pin); // start watering
               break;
      case 3:  
      case 4:
               LL_GPIO_ResetOutputPin(GPIOB, WATER_Pin);// stop watering 
               break;
      case 1990: 
    //           LL_GPIO_ResetOutputPin(GPIOB, SENSOR_Pin); // Enable Sensor
               break;
             
      case 1992: 
      case 1993: 
      case 1994:
      case 1995:
      case 1996:
      case 1997:
      case 1998:
      case 1999:  F1_push(readADC);
                  F1_push(evaluateMoisture);
            

  
  }
 
  count++;
  count%=2000;
}

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

#ifdef  USE_FULL_ASSERT
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
