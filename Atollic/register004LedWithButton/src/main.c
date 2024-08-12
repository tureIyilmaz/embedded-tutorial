
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void GPIO_Config(void);
void delay(int);

int  count = 0;

int main(void)
{
	RCC_Config();
	GPIO_Config();

	while (1)
  {
	  if(GPIOA->IDR & 0x00000001)
	  {
		  while(GPIOA->IDR & 0x00000001);
		  delay(16800000);
		  count++;
	  }

	  if(count % 2 == 0 )
	  {
		  GPIOD->ODR = 0x00000000;
	  }
	  else
	  {
		  GPIOD->ODR = 0x0000F000;
	  }

  }
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 0x00000009;

	GPIOA->MODER |= 0x00000000;
	GPIOA->OTYPER |= 0x00000000;
	GPIOA->PUPDR |= 0x00000002;
	GPIOA->OSPEEDR |= 0x00000003;

	GPIOD->MODER |= 0x55000000;
	GPIOD->OTYPER |= 0x00000000;
	GPIOD->PUPDR |= 0x00000000;
	GPIOD->OSPEEDR |= 0xFF000000;
}

void RCC_Config(void)
{

	RCC->CR |= 0x00030000; // HSE and HSEONRDY ENABLE

	while(!(RCC->CR & 0x00020000)); // HSE on ready flag wait
	RCC->CR |= 0x00080000; // CSS Enable

	RCC->PLLCFGR |= 0x00400000;  // selected HSE
	RCC->PLLCFGR |= 0x00000004;	// PLL_M = 4
	RCC->PLLCFGR |= 0x00002A00; // PLL_N = 168
	RCC->PLLCFGR |= 0x00000000; // PLL_P = 2
	RCC->CFGR |= 0x00000000; // AHB PRESCALER = 1
	RCC->CFGR |= 0x00080000; // APB2 PRESCALER = 2
	RCC->CFGR |= 0x00001400; // APB1 PRESCALER = 4

	RCC->CIR |= 0x00080000; // HSERDY Flag clear
	RCC->CIR |= 0x00800000; // CSS Flag clear
}

void delay(int time)
{
		while(time--);
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
/* TODO, implement your code here */
	return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
	 /* TODO, implement your code here */
	 return -1;
}
