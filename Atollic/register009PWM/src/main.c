
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void GPIO_Config(void);
void TIM_Config(void);

int main(void)
{
	RCC_Config();
	GPIO_Config();
	TIM_Config();

  while (1)
  {

  }
}

void TIM_Config(void)
{
	RCC->APB1ENR |= 0x00000004;

	TIM4->CR1 &= ~(1 << 4); // Upcounter
	TIM4->CR1 &= ~(3 << 5); // Edge-aligned mode
	TIM4->CR1 &= ~(3 << 8); //  tDTS = tCK_INT

	TIM4->CCMR1 &= ~((3 << 0) | (3 << 8)); // CC1 and CC2 channels are configured as output
	TIM4->CCMR1 |= (6 << 4) | (6 << 12);  // PWM mode 1

	TIM4->CCMR2 &= ~((3 << 0) | (3 << 8)); // CC1 and CC2 channels are configured as output
	TIM4->CCMR2 |= (6 << 4) | (6 << 12);  // PWM mode 1

	TIM4->CCER |= 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12; // Capture/Compare 4 output enable

	TIM4->PSC = 8399;
	TIM4->ARR = 9999;

	TIM4->CCR1 = 2499;
	TIM4->CCR2 = 4999;
	TIM4->CCR3 = 7499;
	TIM4->CCR4 = 9999;

	TIM4->CR1 |= 1 << 0;

}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 0x00000008;

	GPIOD->MODER = 0xAA000000; // AF
	GPIOD->AFR[1] |= 0x22220000; // TIM4

}

void RCC_Config(void)
{
	RCC->CR |= 0x00030000; // HSE AND HSERDYON ENABLE
	while(!(RCC->CR & 0x00020000)); // HSE on ready flag wait
	RCC->CR |= 0x00080000;  // CSS active
	RCC->CR |= 0x01000000; // PLL ON
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

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
