/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "adc.h"
#include "sdadc.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Thermal_Comp_ADC.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
#define ENABLE_DIFFERENTIAL_IN		(1)
#define ENABLE_SDADC3				(0)


#define TIMEOUT_CALIB				(10000)

#define SAMPLING_RATE_MS			(1000)

#define BUFF_DIM					(50)

#define RAW_BUFF_DIM				(12500)

/* temperature constants */
#define V_25						(1.43)
#define AVS							(0.0043)
#define LSB_TO_V(x)					((3.3/4095)*x)
#define T_25						(25.0)


int16_t raw_value;
uint8_t finish;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	uint8_t txbuff[BUFF_DIM];
	int32_t rawbuff,rawbuff2,i;
	float temp;

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDADC1_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_SDADC3_Init();

  /* USER CODE BEGIN 2 */

#if ENABLE_DIFFERENTIAL_IN

  /* CHANNEL 8 */
  HAL_SDADC_AssociateChannelConfig(&hsdadc1,
		  SDADC_CHANNEL_8,
		  SDADC_CONF_INDEX_1);

  HAL_SDADC_ConfigChannel(&hsdadc1,
		  SDADC_CHANNEL_8,
		  SDADC_CONTINUOUS_CONV_OFF);

#else
  /* CHANNEL 4 */

//  HAL_SDADC_AssociateChannelConfig(&hsdadc1,
//		  SDADC_CHANNEL_4,
//		  SDADC_CONF_INDEX_0);
//
//  HAL_SDADC_ConfigChannel(&hsdadc1,
//		  SDADC_CHANNEL_4,
//		  SDADC_CONTINUOUS_CONV_OFF);

#endif

#if ENABLE_DIFFERENTIAL_IN
  /* calibration */
  if (HAL_SDADC_CalibrationStart(&hsdadc1,SDADC_CALIBRATION_SEQ_1) != HAL_OK){
	  _Error_Handler(__FILE__,__LINE__);
  }
  /* calibration ADC */
  HAL_ADCEx_Calibration_Start(&hadc1);
#else
  /* calibration */
  if (HAL_SDADC_CalibrationStart(&hsdadc1,SDADC_CALIBRATION_SEQ_1) != HAL_OK){
	  _Error_Handler(__FILE__,__LINE__);
  }
#endif
  /* wait end of calib */
  while ( HAL_SDADC_PollForCalibEvent(&hsdadc1,TIMEOUT_CALIB) == HAL_ERROR);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  HAL_SDADC_AssociateChannelConfig(&hsdadc1,
			  SDADC_CHANNEL_6,
			  SDADC_CONF_INDEX_1);

	  HAL_SDADC_ConfigChannel(&hsdadc1,
			  SDADC_CHANNEL_6,
			  SDADC_CONTINUOUS_CONV_OFF);
	  /* calibration */
	  if (HAL_SDADC_CalibrationStart(&hsdadc1,SDADC_CALIBRATION_SEQ_1) != HAL_OK){
		  _Error_Handler(__FILE__,__LINE__);
	  }
	  /* wait end of calib */
	  while ( HAL_SDADC_PollForCalibEvent(&hsdadc1,TIMEOUT_CALIB) == HAL_ERROR);

	  for (i=0 , rawbuff = 0; i < 1/*RAW_BUFF_DIM*/; i++){
		  HAL_SDADC_Start(&hsdadc1);
		  /* wait end of ADC */
		  while ( HAL_SDADC_PollForConversion(&hsdadc1,TIMEOUT_CALIB) == HAL_ERROR);
		  //raw_value = (int16_t)HAL_SDADC_GetValue(&hsdadc1);
		  //rawbuff = rawbuff + (int32_t)raw_value;
	  }
	  //rawbuff = rawbuff / RAW_BUFF_DIM;


	  HAL_SDADC_AssociateChannelConfig(&hsdadc1,
			  SDADC_CHANNEL_8,
			  SDADC_CONF_INDEX_1);

	  HAL_SDADC_ConfigChannel(&hsdadc1,
			  SDADC_CHANNEL_8,
			  SDADC_CONTINUOUS_CONV_OFF);
	  /* calibration */
	  if (HAL_SDADC_CalibrationStart(&hsdadc1,SDADC_CALIBRATION_SEQ_1) != HAL_OK){
		  _Error_Handler(__FILE__,__LINE__);
	  }
	  /* wait end of calib */
	  while ( HAL_SDADC_PollForCalibEvent(&hsdadc1,TIMEOUT_CALIB) == HAL_ERROR);

	  for (i=0 , rawbuff2 = 0; i < RAW_BUFF_DIM; i++){
		  HAL_SDADC_Start(&hsdadc1);
		  /* wait end of ADC */
		  while ( HAL_SDADC_PollForConversion(&hsdadc1,TIMEOUT_CALIB) == HAL_ERROR);
		  raw_value = (int16_t)HAL_SDADC_GetValue(&hsdadc1);
		  rawbuff2 = rawbuff2 + (int32_t)raw_value;
	  }
	  rawbuff2 = rawbuff2 / RAW_BUFF_DIM;

	  /* TEMPERATURE */
	  HAL_ADC_Start(&hadc1);
	  while (HAL_ADC_PollForConversion(&hadc1,TIMEOUT_CALIB) == HAL_ERROR);
	  raw_value = (int16_t)HAL_ADC_GetValue(&hadc1);
	  temp = LSB_TO_V((float)raw_value);
	  temp = ((V_25 - temp)/AVS)+T_25;
	  temp = roundf(temp);

	  /* temp compensation */
	  rawbuff = Temp_Comp (rawbuff,(int16_t)temp);
	  rawbuff2 = Temp_Comp (rawbuff2,(int16_t)temp);

	  sprintf(txbuff,"%d %d %d\r\n",(int16_t)rawbuff,(int16_t)rawbuff2,(int16_t)temp);
	  HAL_UART_Transmit(&huart2,txbuff,strlen((const char *)txbuff),TIMEOUT_CALIB);
	  HAL_Delay(SAMPLING_RATE_MS);


  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_ADC1
                              |RCC_PERIPHCLK_SDADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.SdadcClockSelection = RCC_SDADCSYSCLK_DIV12;
  PeriphClkInit.Adc1ClockSelection = RCC_ADC1PCLK2_DIV8;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_PWREx_EnableSDADC(PWR_SDADC_ANALOG1);

  HAL_PWREx_EnableSDADC(PWR_SDADC_ANALOG3);

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
