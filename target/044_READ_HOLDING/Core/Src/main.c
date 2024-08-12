/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "modbus_crc.h"
#include "string.h"
#include  "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    SM_Idle,
	SM_CRC,
	SM_SlaveAddress,
	SM_FunctionCode,
	SM_ReadRegisterAdressing,
	SM_Length,
	SM_Data,
        SM_Ready,
	SM_Busy,
	SM_Exception,
        SM_Error,
} SM_State;


typedef enum {
    EC_NotException				=	0x00,
    EC_IllegalFunction 				= 	0x01,
    EC_IllegalDataAddress 			= 	0x02,
    EC_IllegalDataValue 			= 	0x03,
} EC_ExceptionCode;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE      			 256
#define BROADCAST_ADDRESS                          0
#define MySlaveAddress 				   5
#define ReadHoldingRegisters                    0x03
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[BUFFER_SIZE];
uint8_t tx_buffer[BUFFER_SIZE];

volatile uint16_t received_data_length = 0;

SM_State SM_state = SM_Idle;
SM_State SM_next_state;

uint8_t exceptionCode = EC_NotException;

uint8_t SlaveAddress;
uint8_t FunctionCode;
uint16_t crc = 0xFF;

uint8_t idle_detected_flag = 0;

// ReadHoldingRegisters
uint16_t InitialDataAddress;
uint16_t NumberOfData;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        received_data_length = BUFFER_SIZE - hdma_usart1_rx.Instance->CNDTR;

    }
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)rx_buffer, BUFFER_SIZE);
}

void sendData(uint8_t *data) {
    HAL_UART_Transmit(&huart1, data, 8, 1000);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rx_buffer, BUFFER_SIZE);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  	switch (SM_state) {

    	case SM_Idle:
    	{
    		tx_buffer[0]                            =       SlaveAddress;
    		SM_state				= 	SM_FunctionCode;
    		break;
    	}
    	case SM_CRC:
    	{
    		crc					=	crc16(rx_buffer, 8);
    		if(crc != 0){
    			SM_state 			= 	SM_Idle;
    		}
    		else {
    			SM_state 			= 	SM_SlaveAddress;
    			SM_next_state                   =	SM_FunctionCode;
    		}
    		break;
    	}
    	case SM_SlaveAddress:
    	{
    		if(SlaveAddress != BROADCAST_ADDRESS || SlaveAddress != MySlaveAddress) {
    			SM_state 			= 	SM_Idle;
    		}
    		else if(SlaveAddress == BROADCAST_ADDRESS){

    		}
    		else {
    			SM_state 			=	SM_next_state;
    		}
        	  break;
    	}
    	case SM_FunctionCode:
    	{
    		switch(FunctionCode)
    		{
    			case ReadHoldingRegisters:
    			{
        			tx_buffer[1] 	= 	ReadHoldingRegisters;
                                SM_state 	= 	SM_Length;
    			}
    			default:
    			{
                                exceptionCode   = 	EC_IllegalFunction;
                                SM_state 	= 	SM_Exception;
    			}
    		}

    		break;
    	}
    	case SM_Length:
    	{
    		InitialDataAddress 		= 	((rx_buffer[2]<<8)|rx_buffer[3]);
    		NumberOfData 			=	((rx_buffer[4]<<8)|rx_buffer[5]);

    		if((InitialDataAddress + NumberOfData * 2) >= 65535){
                exceptionCode                   = 	EC_IllegalDataValue;
                SM_state                        = 	SM_Exception;
    		}
    		else {
                SM_state                        = SM_ReadRegisterAdressing;
    		}

    		break;
    	}
      	case SM_ReadRegisterAdressing:
      	{

      		break;
      	}
      	case SM_Data:
      	{
    		if((NumberOfData < 1) || (NumberOfData > 123)){
    			exceptionCode           = 	EC_IllegalDataValue;
    			SM_state 		= 	SM_Exception;
    		}
    		else{
    			SM_state		= 	SM_Ready;
    		}
      		break;
      	}
      	case SM_Ready:
      	{
              HAL_Delay(4);
              sendData(tx_buffer);
              SM_state 				= 	SM_next_state;
              break;
      	}
      	case SM_Busy:
      	{
      		break;
      	}


          case SM_Exception:
          {
          	HAL_Delay(4);
          	tx_buffer[0]			=   SlaveAddress;
          	tx_buffer[1]			=	FunctionCode ;
          	tx_buffer[2]			=	exceptionCode;
          	crc                             =	crc16(tx_buffer, 3);
          	tx_buffer[3] 			= 	crc & 0xFF;
          	tx_buffer[4] 			= 	(crc >> 8)&0xFF;
          	sendData(tx_buffer);
          	exceptionCode                   = 	EC_NotException;
          	SM_state 			= 	SM_Idle;
          	break;
          }
          case SM_Error:
          {
          	SM_state                        = 	SM_Idle;
              break;
          }
          default:
          {
              SM_state 				= 	SM_Error;
              break;
          }

      }


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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
