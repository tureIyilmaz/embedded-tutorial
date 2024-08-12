
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void RCC_Config(void);
void GPIO_Config(void);
void delay(int);

int count = 0;

int main(void)
{

	RCC_Config();
	GPIO_Config();

  while (1)
  {
	  if( count == 0)
	  {
		  GPIOA->ODR = 0x00000040;
	  }


	  if(GPIOE->IDR & 0x00000001)
	  {
		  while(GPIOE->IDR & 0x00000001);
		  delay(1680000);

		  count++;

		  switch(count)
		  {
		  	  case 0:
		  		  	  GPIOA->ODR = 0x00000040;
		  		  	  GPIOD->ODR = 0x00000000;
		  		  	  break;
		  	  case 1:
		  		  	  GPIOA->ODR = 0x00000079;
		  		  	  GPIOD->ODR = 0x00001000;
		  		  	  break;
		  	  case 2:
		  		  	  GPIOA->ODR = 0x00000024;
		  		  	  GPIOD->ODR = 0x00002000;
		  		  	  break;
		  	  case 3:
		  		  	  GPIOA->ODR = 0x00000030;
		  		  	  GPIOD->ODR = 0x00004000;
		  		  	  break;
		  	  case 4:
		  		  	  GPIOA->ODR = 0x00000019;
		  		  	  GPIOD->ODR = 0x00008000;
		  		  	  break;
		  	  case 5:
		  			  GPIOA->ODR = 0x00000012;
		  			  GPIOD->ODR = 0x00003000;
		  			  break;
		  	  case 6:
		  			  GPIOA->ODR = 0x00000002;
		  			  GPIOD->ODR = 0x0000C000;
		  			  break;
		  	  case 7:
		  		  	  GPIOA->ODR = 0x00000078;
		  		  	  GPIOD->ODR = 0x0000F000;
		  		  	  break;
		  	  case 8:
		  			  GPIOA->ODR = 0x00000000;
		  			  GPIOD->ODR = 0x00000000;
		  			  break;
		  	  default:
		  		  	  GPIOA->ODR = 0x00000010;
		  		  	  GPIOD->ODR = 0x00001000;
		  		  	  delay(1680000);
		  		  	  GPIOD->ODR = 0x00002000;
		  		  	  delay(1680000);
		  		  	  GPIOD->ODR = 0x00004000;
		  		  	  delay(168000);
		  		  	  GPIOD->ODR = 0x00002000;
		  		  	  delay(1680000);
		  		  	  GPIOD->ODR = 0x0000F000;
		  		  	  count = -1;
		  }

	  }
  }
}

void RCC_Config(void)
{
	RCC->CR |=  0x000B0000;
	RCC->PLLCFGR |= 0x00402A04;
	RCC->CFGR |= 0x00009400;
	RCC->CIR |= 0x00880000;
}
void GPIO_Config(void)
{
	RCC->AHB1ENR |= 0x00000019;

	GPIOE->MODER |= 0x00000000;
	GPIOE->OTYPER |= 0x00000000;
	GPIOE->PUPDR |= 0x00000002;
	GPIOE->OSPEEDR |= 0x00000003;

	GPIOD->MODER |= 0x55000000;
	GPIOD->OTYPER |= 0x00000000;
	GPIOD->PUPDR |= 0x00000000;
	GPIOD->OSPEEDR |= 0xFF000000;

	GPIOA->MODER |= 0x00001555;
	GPIOA->OTYPER |= 0x00000000;
	GPIOA->PUPDR |= 0x00000000;
	GPIOA->OSPEEDR |= 0x00003FFF;


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

