
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void DAC1_Config(void);
void GPIO_Config(void);
void delay(int);

int i = 0;

int main(void)
{
	RCC_Config();
	DAC1_Config();
	GPIO_Config();

	  GPIOA->ODR = 0x00000000;
  while (1)
  {
	  for(i = 0; i < 4096; i++)
	  {
		  DAC->DHR12R1 = i;
		  delay(16800);
	  }
	  i = 0;
  }
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 0x00000001;


	GPIOA->MODER |= 0x00000005;
	GPIOA->OTYPER |= 0x00000000;
	GPIOA->PUPDR |= 0x00000000;
	GPIOA->OSPEEDR |= 0x0000000F;
}

void DAC1_Config()
{
	RCC->APB1ENR |= 0x00000001;
	RCC->APB1ENR |= 0x20000000;

	DAC->CR |= (1 << 0); // DAC channel1 enabled
	DAC->SWTRIGR &= ~(1 << 0);  // Software trigger disabled
	DAC->DHR12R1 |= 0x00000000; // DAC channel1 12-bit right-aligned data
}

void RCC_Config(void)
{
	RCC->CR |= 0x00030000; // HSE AND HSERDYON ENABLE
	while(!(RCC->CR & 0x00020000)); // HSE on ready flag wait
	RCC->CR |= 0x00080000;  // CSS active
	RCC->PLLCFGR |=
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
