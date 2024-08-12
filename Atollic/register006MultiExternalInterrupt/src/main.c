

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void GPIO_Config(void);
void EXTI_Config(void);
void RCC_Config(void);
void delay(int);

int main(void)
{
	GPIO_Config();
	EXTI_Config();
	RCC_Config();

  while (1)
  {
	  GPIOD->ODR = 0x0000F000;
  }
}

void EXTI0_IRQHandler()
{
	if(EXTI->PR & 0x00000001)
	{
		GPIOD->ODR = 0x00001000;
		delay(33600000);

		EXTI->PR = 0x00000001;
	}
}

void EXTI1_IRQHandler()
{
	if(EXTI->PR & 0x00000002)
	{
		GPIOD->ODR = 0x00002000;
		delay(33600000);

		EXTI->PR = 0x00000002;
	}
}

void EXTI2_IRQHandler()
{
	if(EXTI->PR & 0x00000004)
	{
		GPIOD->ODR = 0x00004000;
		delay(33600000);

		EXTI->PR =0x00000004;
	}
}

void GPIO_Config(void)
{
	RCC->AHB1ENR = 0x00000009;


	GPIOD->MODER|= 0x55000000;
	GPIOD->OTYPER = 0x00000000;
	GPIOD->PUPDR = 0x00000000;
	GPIOD->OSPEEDR = 0xFF000000;
}

void EXTI_Config(void)
{
	RCC->APB2LPENR = 0x00004000;

	SYSCFG->EXTICR[0] = 0x00000000;

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);

	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_SetPriority(EXTI2_IRQn, 2);

	EXTI->IMR = 0x00000007;
	EXTI->RTSR = 0x00000007;

}

void RCC_Config(void)
{
	RCC->CR |=  0x00030000;
	while(!(RCC->CR &  0x00020000)); // HSEONRDY FLAG WAÝT
	RCC->CR |=  0x00080000;
	RCC->PLLCFGR |= 0x00402A04;
	RCC->CFGR |= 0x00009400;
	RCC->CIR |= 0x00880000;
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
