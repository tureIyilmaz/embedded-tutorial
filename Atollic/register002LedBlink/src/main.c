
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


void RCC_Config(void);
void GPIO_Config(void);
void delay(uint32_t);

extern uint32_t SystemCoreClock;
uint32_t systemClock;

int main(void)
{

	RCC_Config();
	SystemCoreClockUpdate();
	systemClock = SystemCoreClock;
	GPIO_Config();

  while (1)
  {
	  GPIOD->ODR |= (-1 << 12);
	  delay(16800000);
	  GPIOD->ODR &= ~(-1 << 12);
	  delay(16800000);
  }
}


void RCC_Config(void)
{
	RCC->CR &=  0x00000083; // Reset value
	RCC->CR &=  ~(1 << 0); // HSI OFF
	RCC->CR |= (1 << 16); // HSE ON

	while(!(RCC->CR & 1 <<17)); //wait HSE active
	RCC->CR |= (1 << 19); // CSS GUVENLIK

	RCC->PLLCFGR =  0x00000000; // tum bitler 0 ve bu sayede PLL_P = 2
	RCC->PLLCFGR |= (1 << 22);  // selected HSE
	RCC->PLLCFGR |= (4 << 0);	// PLL_M = 4
	RCC->PLLCFGR |= (168 << 6); // PLL_N = 168

	RCC->CR |= (1 << 24); // PLL ON
	while(!(RCC->CR & 1 << 25 )); // wait PLL active

	RCC->CFGR |= (2 << 0);
	while(!(RCC->CFGR & (2 << 0)));

}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= (1 << 3);

	GPIOD->MODER |= (85 << 24); // GPIOD 12-13-14-15 OUTPUT MODE ( LEDS )
	GPIOD->OTYPER &= ~(-1 << 12); // PUSH-PULL
	GPIOD->OSPEEDR |= (-1 << 24); //  VERY HIGH-SPEED
	GPIOD->PUPDR &= ~(-1 << 24); // NO PULL-UP, NO PULL-DOWN

}

void delay(uint32_t time)
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
