
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
uint8_t Read_ADC(void);

uint8_t data;

int main(void)
{
	RCC_Config();
	GPIO_Config();
	ADC_Config();

  while (1)
  {
	  data = Read_ADC();
	  if(data == 0)
	  {
		  GPIOD->ODR = 0x00000040;
	  }
	  else if(data < 16)
	  {
		  GPIOD->ODR = 0x00000079;
	  }
	  else if(data > 16 && data < 32)
	  {
		  GPIOD->ODR = 0x00000024;
	  }
	  else if(data > 32 && data < 48)
	  {
		  GPIOD->ODR = 0x00000030;
	  }
	  else if(data > 48 && data < 64)
	  {
		  GPIOD->ODR = 0x00000019;
	  }
	  else if(data > 64 && data < 80)
	  {
		  GPIOD->ODR = 0x00000012;
	  }
	  else if(data > 80 && data < 96)
	  {
		  GPIOD->ODR = 0x00000002;
	  }
	  else if(data > 96 && data < 112)
	  {
		  GPIOD->ODR = 0x00000078;
	  }
	  else if(data > 112 && data < 128)
	  {
		  GPIOD->ODR = 0x00000000;
	  }
	  else
	  {
		  GPIOD->ODR = 0x00000010;
	  }
  }
}

uint8_t Read_ADC(void)
{
	ADC1->CR2 |= 0x40000000;
	while(!(ADC1->SR & (2 << 0)));

	return (ADC1->DR);
}

void ADC_Config(void)
{
	RCC->APB2ENR |= 0x00000100;

	ADC1->CR1 |= 0x02000000; // 8bit Resolution
	ADC1->CR2 |= 0x00000001; // ADC Enable
	ADC1->SMPR2 |= 0x00000003; // 56 CC
	ADC->CCR |= 0x00010000;


}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 0x00000009;

	GPIOA->MODER |= 0x00000003;
	GPIOA->OTYPER |= 0x00000000;
	GPIOA->PUPDR |= 0x00000000;
	GPIOA->OSPEEDR |= 0x00000003;

	GPIOD->MODER |= 0x00001555;
	GPIOD->OTYPER |= 0x00000000;
	GPIOD->PUPDR |= 0x00000000;
	GPIOD->OSPEEDR |= 0x00003FFF;
}

void RCC_Config(void)
{
	RCC->CR |= 0x000B0000;
	while(!(RCC->CR & 0x00020000));
	RCC->PLLCFGR |= 0x00402A04;
	RCC->CFGR |= 0x00009400;
	RCC->CIR |= 0x00880000;

}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}


