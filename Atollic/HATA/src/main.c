#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void GPIO_Config(void);
void ADC_Config(void);
void TIM_Config(void);
uint8_t Read_ADC(void);
void delay_ms(uint32_t);


GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
TIM_OCInitTypeDef TIM_OC_InitStruct;
uint8_t adc_value;
uint32_t count;

int main(void)
{
	GPIO_Config();
	ADC_Config();
	TIM_Config();


  while (1)
  {
	  adc_value =  Read_ADC();

	  TIM_OC_InitStruct.TIM_Pulse =  100;
	  TIM_OC1Init(TIM4, &TIM_OC_InitStruct);
	  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	  delay_ms(20);
	  TIM_OC_InitStruct.TIM_Pulse = 75;
	  TIM_OC2Init(TIM4, &TIM_OC_InitStruct);
	  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	  delay_ms(20);
	  TIM_OC_InitStruct.TIM_Pulse = 50;
	  TIM_OC3Init(TIM4, &TIM_OC_InitStruct);
	  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	  delay_ms(20);
	  TIM_OC_InitStruct.TIM_Pulse = 25;
	  TIM_OC4Init(TIM4, &TIM_OC_InitStruct);
	  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	  delay_ms(20);
  }
}

uint8_t Read_ADC(void)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADC1);
}

void TIM_Config(void)
{
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 99;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 83;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM4, ENABLE);

	TIM_OC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_InitStruct.TIM_OutputState = ENABLE;

	SysTick_Config(SystemCoreClock / 1000);
}

void ADC_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4 ;

	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_8b;

	ADC_Init(ADC1, &ADC_InitStruct);
	ADC_Cmd(ADC1, ENABLE);

}

void GPIO_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

}

void delay_ms(uint32_t time)
{
	time = count;
	while(count);
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
