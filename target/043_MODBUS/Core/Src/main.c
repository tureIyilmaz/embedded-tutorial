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
    SM_SlaveAddress,
    SM_FunctionCode,
    SM_ReadBooleanAdressing,
    SM_BooleanLength,
    SM_ReadRegisterAdressing,
    SM_RegisterLength,
    SM_WriteBooleanAdressing,
    SM_BooleanValue,
    SM_WriteRegisterAdressing,
    SM_RegisterValue,
    SM_MultipleRegisterValue,
    SM_CRC,
    SM_Ready,
	SM_Exception,
    SM_Error,
} SM_State;


typedef enum {
EC_NotException				=	0x00,
    EC_IllegalFunction 			= 	0x01,
    EC_IllegalDataAddress 		= 	0x02,
    EC_IllegalDataValue 		= 	0x03,
    EC_SlaveDeviceFailure 		= 	0x04,
    EC_Acknowledge 			= 	0x05,
    EC_SlaveDeviceBusy 			= 	0x06,
    EC_NegativeAcknowledge 		= 	0x07,
    EC_MemoryParityError 		= 	0x08,
    EC_GatewayPathUnavailable           = 	0x10,
    EC_GatewayTargetDeviceFailed 	= 	0x11,
} EC_ExceptionCode;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE      		256
#define BROADCAST_ADDRESS     		  0

#define SlaveAddress                      5

#define ReadCoils       		0x01
#define ReadHoldingRegisters    	0x03
#define WriteASingleCoil     		0x05
#define WriteSingleRegister   		0x06
#define WriteMultipleRegisters   	0x10


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */
uint16_t rx_buffer[BUFFER_SIZE];
uint8_t tx_data[BUFFER_SIZE];

volatile uint16_t received_data_length = 0;
SM_State SM_state = SM_Idle;
SM_State SM_next_state;
uint8_t exceptionCode = EC_NotException;



uint8_t FunctionCode;
uint8_t temp[2], temp2[2];
uint16_t crc = 0xFF;

// ReadCoils
uint16_t AddressOfFirstBoolean;
uint16_t TotalNumberOfBooleans;
//WriteASingleCoil
uint16_t DataAddressBoolean;
uint16_t valuetoWriteBoolean;
// ReadHoldingRegisters
uint16_t InitialDataAddress;
uint16_t NumberOfData;
uint16_t NumberOfDataBytes;

//WriteSingleRegister
uint16_t DataAddressRegister;
uint16_t valuetoWrite;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CRC_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        received_data_length = BUFFER_SIZE - hdma_usart1_rx.Instance->CNDTR;

    }

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
  MX_CRC_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*) rx_buffer, BUFFER_SIZE);
/*
	FunctionCode 				= 	0x10;

	if(FunctionCode == ReadCoils)
	{
		AddressOfFirstBoolean	= 	0x0000;
		TotalNumberOfBooleans	= 	2001;
	}

	if(FunctionCode == ReadHoldingRegisters)
	{
		InitialDataAddress 		= 	0x006B;
		NumberOfData 			= 	0x0003;
	}

	if(FunctionCode == WriteASingleCoil)
	{
		DataAddressBoolean 		= 	0x0431;
		valuetoWriteBoolean		=	0x00FF;
	}

	if(FunctionCode == WriteSingleRegister)
	{
		DataAddressRegister 	= 	0x0001;
		valuetoWrite 			= 	0x0003;
	}


	if(FunctionCode == WriteMultipleRegisters)
	{
		InitialDataAddress 		= 	0x0001;
		NumberOfData 			= 	0x0020;
	}
	*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    	switch (SM_state) {

            case SM_Idle:
            {

            	if(rx_buffer[0] == SlaveAddress)
            	{
            		rx_buffer[1] = FunctionCode;

            		if(FunctionCode == ReadCoils)
            		{
            			AddressOfFirstBoolean	= 	0x0000;
            			TotalNumberOfBooleans	= 	2001;
            			SM_state		= 	SM_SlaveAddress;
            		}

            		if(FunctionCode == ReadHoldingRegisters)
            		{
            			InitialDataAddress 	= 	0x006B;
            			NumberOfData 		= 	0x0003;
            			SM_state		= 	SM_SlaveAddress;
            		}

            		if(FunctionCode == WriteASingleCoil)
            		{
            			DataAddressBoolean 	= 	0x0431;
            			valuetoWriteBoolean	=	0x00FF;
            			SM_state		= 	SM_SlaveAddress;
            		}

            		if(FunctionCode == WriteSingleRegister)
            		{
            			DataAddressRegister     = 	0x0001;
            			valuetoWrite 		= 	0x0003;
            			SM_state		= 	SM_SlaveAddress;
            		}


            		if(FunctionCode == WriteMultipleRegisters)
            		{
            			InitialDataAddress 	= 	0x0001;
            			NumberOfData 		= 	0x0020;
            			SM_state		= 	SM_SlaveAddress;
            		}
            	}
                break;
            }

            case SM_SlaveAddress:
            {
                if 		(SlaveAddress == BROADCAST_ADDRESS) {
                    SM_state 			= 	SM_Idle;
                }
                else if (SlaveAddress > 0 && SlaveAddress <= 247) {
                    tx_data[0] 			=	SlaveAddress;
                    SM_state 			= 	SM_FunctionCode;
                }
                else {
                	SM_state 		= 	SM_Idle;
                }
                break;
            }

            case SM_FunctionCode:
            {
            	if		(FunctionCode == ReadCoils) {
                    tx_data[1] 			= 	ReadCoils;
                    SM_state 			= 	SM_ReadBooleanAdressing;
            	}
            	else if (FunctionCode == ReadHoldingRegisters) {
                    tx_data[1] 			= 	ReadHoldingRegisters;
                    SM_state 			= 	SM_ReadRegisterAdressing;
                }
            	else if (FunctionCode == WriteASingleCoil) {
                    tx_data[1] 			= 	WriteASingleCoil;
                    SM_state 			= 	SM_WriteBooleanAdressing;
                }
                else if (FunctionCode == WriteSingleRegister) {
                    tx_data[1] 			= 	WriteSingleRegister;
                    SM_state 			= 	SM_WriteRegisterAdressing;
                }
                else if (FunctionCode == WriteMultipleRegisters) {
                    tx_data[1] 			= 	WriteMultipleRegisters;
                    SM_state 			= 	SM_ReadRegisterAdressing;
                }
                else {
                    exceptionCode 		= 	EC_IllegalFunction;
                    SM_state 			= 	SM_Exception;
                }
                break;
            }

            case SM_ReadBooleanAdressing:
            {
        		tx_data[2] 		= 	(AddressOfFirstBoolean >> 8)&0xFF;
        		tx_data[3] 		= 	AddressOfFirstBoolean & 0xFF;
        		SM_state 		= 	SM_BooleanLength;
        		SM_next_state 		= 	SM_CRC;
            }

        	case SM_BooleanLength:
        	{
        		if((AddressOfFirstBoolean + TotalNumberOfBooleans) < 65535)
        		{
        			tx_data[4] 		= 	(TotalNumberOfBooleans >> 8)&0xFF;
            		tx_data[5] 			= 	TotalNumberOfBooleans & 0xFF;
            		SM_state 			= 	SM_next_state;

            		if((TotalNumberOfBooleans < 1) || (TotalNumberOfBooleans > 2000))
            		{
            			exceptionCode 	= 	EC_IllegalDataValue;
            			SM_state 		= 	SM_Exception;
            		}
        		}
        		else
        		{
                    exceptionCode 		= 	EC_IllegalDataAddress;
                    SM_state 			= 	SM_Exception;
        		}

        		break;
        	}


        	case SM_ReadRegisterAdressing:
        	{

        		tx_data[2] 				= 	(InitialDataAddress >> 8)&0xFF;
        		tx_data[3] 				= 	InitialDataAddress & 0xFF;
        		SM_state 				= 	SM_RegisterLength;
        		SM_next_state 			= 	SM_CRC;
        		break;
        	}

        	case SM_RegisterLength:
        	{
        		if(FunctionCode == ReadHoldingRegisters)
        		{
            		NumberOfDataBytes 	= 	NumberOfData * 2;
            		if((InitialDataAddress + NumberOfDataBytes) < 65535)
            		{
            			tx_data[4] 		= 	(NumberOfData >> 8)&0xFF;
                		tx_data[5] 		= 	NumberOfData & 0xFF;
                		SM_state 		= 	SM_next_state;

                		if((NumberOfData < 1) || (NumberOfData > 125))
                		{
                			exceptionCode	= 	EC_IllegalDataValue;
                			SM_state 	= 	SM_Exception;
                		}
            		}
            		else
            		{
                        exceptionCode 	= 	EC_IllegalDataAddress;
                        SM_state 	= 	SM_Exception;
            		}

        		}
        		else if(FunctionCode == WriteMultipleRegisters)
        		{
            		NumberOfDataBytes 	= 	NumberOfData * 2;
            		if((InitialDataAddress + NumberOfDataBytes) < 65535)
            		{
            			tx_data[4] 		= 	(NumberOfData >> 8)&0xFF;
                		tx_data[5] 		= 	NumberOfData & 0xFF;
                		SM_state 		= 	SM_MultipleRegisterValue;

                		if((NumberOfData < 1) || (NumberOfData > 123))
                		{
                			exceptionCode 	= 	EC_IllegalDataValue;
                			SM_state 		= 	SM_Exception;
                		}
            		}
            		else
            		{
                        exceptionCode 	= 	EC_IllegalDataAddress;
                        SM_state 		= 	SM_Exception;
            		}
        		}


        		break;
        	}

        	case SM_WriteBooleanAdressing:
        	{
        		tx_data[2] 			= 	DataAddressBoolean & 0xFF;
        		tx_data[3] 			= 	(DataAddressBoolean >> 8)&0xFF;
        		SM_state 			= 	SM_BooleanValue;
        		SM_next_state 			= 	SM_CRC;
        		break;
        	}

        	case  SM_BooleanValue:
        	{
        		if(valuetoWriteBoolean == 0x00FF || valuetoWriteBoolean == 0x0000) {
            		tx_data[4] 			= 	valuetoWriteBoolean & 0xFF;
            		tx_data[5] 			= 	(valuetoWriteBoolean >> 8)&0xFF;
            		SM_state 			= 	SM_next_state;
        		}
        		else {
        			exceptionCode           = 	EC_IllegalDataValue;
        			SM_state 		= 	SM_Exception;
        		}

        		break;
        	}

        	case SM_WriteRegisterAdressing:
        	{

        		tx_data[2] 			= 	DataAddressRegister & 0xFF;
        		tx_data[3] 			= 	(DataAddressRegister >> 8)&0xFF;
        		SM_state 			= 	SM_RegisterValue;
        		SM_next_state 			= 	SM_CRC;
        		break;
        	}

        	case SM_RegisterValue:
        	{
        		tx_data[4] 			= 	valuetoWrite & 0xFF;
        		tx_data[5] 			= 	(valuetoWrite >> 8)&0xFF;
        		SM_state 			= 	SM_next_state;
        		break;
        	}

        	case SM_MultipleRegisterValue:
        	{

        		for(int i = 0; i < NumberOfData; i++)
        		{
        			tx_data[6 + (i * 2)] = i;
        			tx_data[7 + (i * 2)] = i + 1;
        		}
        		SM_state 				=	SM_Ready;
        		break;
        	}

            case SM_CRC:
            {
            	HAL_Delay(5);

            	if(FunctionCode == WriteMultipleRegisters)
            	{

                	crc				=	crc16(tx_data, 8 + NumberOfDataBytes);
                    tx_data[8 + (NumberOfData * 2)]	= 	crc & 0xFF;
                    tx_data[9 + (NumberOfData * 2)] 	= 	(crc >> 8)&0xFF;

            	}
            	else
            	{
                	crc			=	crc16(tx_data, 6);
                    tx_data[6] 			= 	crc & 0xFF;
                    tx_data[7] 			= 	(crc >> 8)&0xFF;

            	}
                SM_state 			= 	SM_Ready;
                SM_next_state 			= 	SM_Idle;
                break;
            }

            case SM_Ready:
            {
            	HAL_Delay(4);
                sendData(tx_data);
                SM_state 			= 	SM_next_state;
                break;
            }

            case SM_Exception:
            {
            	tx_data[0]			=   SlaveAddress;
            	tx_data[1]			=	FunctionCode;
            	tx_data[2]			=	exceptionCode;
            	crc				=	crc16(tx_data, 3);
                tx_data[3] 			= 	crc & 0xFF;
                tx_data[4] 			= 	(crc >> 8)&0xFF;
            	sendData(tx_data);
            	exceptionCode 			= 	EC_NotException;
            	SM_state 			= 	SM_Idle;
            	break;
            }

            case SM_Error:
            {
            	SM_state 			= 	SM_Idle;
                break;
            }
            default:
            {
                SM_state 			= 	SM_Error;
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
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
    while (1) {
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
