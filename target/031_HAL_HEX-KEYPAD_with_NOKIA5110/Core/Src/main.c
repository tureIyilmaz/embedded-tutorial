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
#include "stdbool.h"
#include "nokia5110.h"
#include "keypad4x3.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYPAD_IN_PORT   GPIOD
#define KEYPAD_OUT_PORT  GPIOD

#define KEYPAD_IN_PIN_0  GPIO_PIN_0
#define KEYPAD_IN_PIN_1  GPIO_PIN_1
#define KEYPAD_IN_PIN_2  GPIO_PIN_2
#define KEYPAD_IN_PIN_3  GPIO_PIN_3

#define KEYPAD_OUT_PIN_0 GPIO_PIN_4
#define KEYPAD_OUT_PIN_1 GPIO_PIN_5
#define KEYPAD_OUT_PIN_2 GPIO_PIN_6
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

/* USER CODE BEGIN PV */
Keypad4x3_WiresTypeDef keypadWires;
bool keys[12];
char key1, key2, key3, key4, key5, key6, key7, key8, key9, keyS, key0, keyY;
char correctPassword[] = {2, 3, 4, 5};
char enteredPassword[4] = {0};
int passwordIndex = 0;

uint8_t frameBuff[504];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
bool checkPassword(char password[]);
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  keypadWires.IN_Port = KEYPAD_IN_PORT;
  keypadWires.IN_PIN_0 = KEYPAD_IN_PIN_0;
  keypadWires.IN_PIN_1 = KEYPAD_IN_PIN_1;
  keypadWires.IN_PIN_2 = KEYPAD_IN_PIN_2;
  keypadWires.IN_PIN_3 = KEYPAD_IN_PIN_3;

  keypadWires.OUT_Port = KEYPAD_OUT_PORT;
  keypadWires.OUT_PIN_0 = KEYPAD_OUT_PIN_0;
  keypadWires.OUT_PIN_1 = KEYPAD_OUT_PIN_1;
  keypadWires.OUT_PIN_2 = KEYPAD_OUT_PIN_2;

  Keypad4x3_Init(&keypadWires);
  nokia5110_Init();
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
  bool keys[12] = {false};
  nokia5110_WriteString("password");
  nokia5110_Update();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  Keypad4x3_ReadKeypad(keys);
	  key1 = keys[0];
	  key2 = keys[4];
	  key3 = keys[8];
	  key4 = keys[2];
	  key5 = keys[6];
	  key6 = keys[10];
	  key7 = keys[1];
	  key8 = keys[5];
	  key9 = keys[9];
	  keyY = keys[3];
	  key0 = keys[7];
	  keyS = keys[11];


	  char Keys[12] ={key1,key2,key3,key4,key5,key6,key7,key8,key9,keyY,key0,keyS};



	    if (Keys[0] || Keys[1] || Keys[2] || Keys[3] || Keys[4] || Keys[5] ||
	        Keys[6] || Keys[7] || Keys[8] || Keys[9] || Keys[10] || Keys[11])
	    {

	      for (int i = 0; i < 12; i++)
	      {
	        if (Keys[i])
	        {
	          enteredPassword[passwordIndex] = i + 1; // +1 because the keypad numbers start from 1
	          passwordIndex += 1;
	          break;
	        }
	      }


	      if (passwordIndex > 0 && passwordIndex < 4)
	             {
	                 char passwordString[5]; // Şifre 4 basamaklı ve son karakter için '\0' gerekiyor
	                 for (int i = 0; i < passwordIndex; i++)
	                 {
	                     passwordString[i] = enteredPassword[i] + '0'; // Karakter dizisi olarak dönüştür
	                 }
	                 passwordString[passwordIndex] = '\0'; // Son karakteri '\0' olarak ayarla

	                 displayPassword(passwordString, passwordIndex);

	             }

	      // Check if the full password has been entered
	      if (passwordIndex == 4)
	      {

	        // Check the entered password with the correct password
	        if (checkPassword(enteredPassword))
	        {

	          nokia5110_WriteString("SUCCESS");
	        }
	        else
	        {

	          nokia5110_WriteString("FAIL");
	        }

	        nokia5110_Update(); // Update the LCD screen
	        HAL_Delay(2000); // Wait for a short duration before resetting the password entry
	        passwordIndex = 0; // Reset the password entry for the next attempt
	        nokia5110_Clear(); // Clear the LCD screen after displaying the result
	        HAL_Delay(5000);
	        nokia5110_Update();
	      }
	    }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_15
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD8 PD9 PD10 PD15
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_15
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
bool checkPassword(char password[]) {
    uint8_t correctPassword[] = {2, 3, 4, 5};

    for (int i = 0; i < 4; i++) {
        if (password[i] != correctPassword[i]) {
            return false;
        }
    }

    return true;
}

void displayPassword(char password[], int length)
{
    // Şifreyi ekrana yazdır
    nokia5110_Clear(); // Ekrana temizle
    nokia5110_WriteString(password); // Şifreyi yazdır
    nokia5110_Update(); // Ekrana yazılanları güncelle
    HAL_Delay(1000);
    nokia5110_Clear();
    nokia5110_Update();
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
