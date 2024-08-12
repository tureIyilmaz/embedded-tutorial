
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void TIM_Config(void);

uint16_t count = 0;

int main(void)
{
	RCC_Config();
	TIM_Config();
  while (1)
  {
	  count = TIM2->CNT;
  }
}

void TIM_Config(void)
{
	RCC->APB1ENR |= 0x00000001; // TIM2 ENABLE

	TIM2->CR1 |= (1 << 0);  // Counter enabled
	TIM2->CR1 &= ~(1 << 4);  // : Counter used as upcounter
	TIM2->CR1 &= ~(3 << 5);  // Edge-aligned mode
	TIM2->CR1 &= ~(3 << 8); // tDTS = tCK_INT
	TIM2->SMCR &= ~(7 << 0); // Slave mode disabled
	TIM2->EGR |= (1 << 0);  //  Re-initialize the counter
	TIM2->PSC = 41999;
	TIM2->ARR = 4000;

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
