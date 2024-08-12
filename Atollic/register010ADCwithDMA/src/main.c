
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/* Private macro */
#define BufferLength 1

/* Private variables */
uint32_t adc_value[BufferLength];
/* Private function prototypes */
void RCC_Config(void);
void GPIO_Config(void);
void ADC_Config(void);
void DMA_Config(void);

int main(void)
{
	RCC_Config();
	GPIO_Config();
	ADC_Config();
	DMA_Config();
	ADC1->CR2|= 0x40000000; // Starts conversion of regular channels
  while (1)
  {

  }
}
void DMA_Config(void)
{
	RCC->AHB1ENR |= 1 << 22; // DMA2 clock enabled

	while ((DMA2_Stream0->CR & 0x00000001) == 1); // wait for Stream0 to be 0
	DMA2_Stream0->PAR = (uint32_t) &ADC1->DR;
	DMA2_Stream0->M0AR = &adc_value;
	DMA2_Stream0->CR &= ~(3 << 6); // Peripheral-to-memory
	DMA2_Stream0->CR |= 1 << 8; // Circular mode enabled
	DMA2_Stream0->CR &= ~(1 << 9); // Peripheral address pointer is fixed
	DMA2_Stream0->CR |= 1 << 10; //  Memory address pointer is incremented after each data transfer
	DMA2_Stream0->CR &= ~(3 << 11); // Peripheral data size 8bit
	DMA2_Stream0->CR &= ~(3 << 13); // Peripheral data size 8bit
	DMA2_Stream0->CR |= 3 << 16; //  Priority level: very high
	DMA2_Stream0->CR &= ~(7 << 25); // Channel 0
	DMA2_Stream0->NDTR = BufferLength; // single channel
	DMA2_Stream0->FCR |= 1 << 0; // FIFO threshold : 1/2
	DMA2_Stream0->CR |= 1 << 0; // Stream0 enable

}

void ADC_Config(void)
{
	RCC->APB2ENR |= 0x00000100; // ADC1 ENABLE

	ADC1->CR1 |= 0x00000100; // Scan mode enabled
	ADC1->CR1 |= 0x02000000; // 8-bit
	ADC1->CR2 |= 0x00000001; // ADC Enable
	ADC1->CR2 |= 0x00000002; // Continuous conversion mode
	ADC1->CR2 |= 0x00000100; //  DMA mode enabled
	ADC1->CR2 |= 0x00000200; //  DMA requests are issued as long as data are converted and DMA=1
	ADC1->CR2 |= 0x00000400; //  The EOC bit is set at the end of each regular conversion.
	ADC1->SMPR2 |= 0x00000003; // 56 cycles
	ADC1->SQR1 &= ~(15 << 20); //  1 conversion
	ADC1->SQR3 &= ~(31 << 0); //   1st conversion in regular sequence = 0;
	ADC->CCR |= 0x00010000; // PCLK2 divided by 4
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 1 << 0;

	GPIOA->MODER |= 3 << 0;
	GPIOA->OSPEEDR |= 3 << 0;

}

void RCC_Config(void)
{
	RCC->CR |= 0x00010000; // HSE ENABLE
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
