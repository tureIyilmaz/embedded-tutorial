/*
 * max6675k.c
 *
 *  Created on: Jul 26, 2023
 *      Author: turel
 */

#include "max6675k.h"



uint8_t MAX6675K_ReadRegisters(uint16_t* reg)
{
	HAL_StatusTypeDef HAL_Status = HAL_ERROR;
	uint8_t temp[2] ={0};
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_Status = HAL_SPI_Receive(&hspi1, temp, 2, 100);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

	if(HAL_Status == HAL_OK)
	{
		if(temp[1] & 0x04)
		{
			return MAX6675K_ERROR;
		}
		*reg = (uint16_t)(temp[1] >> 3);
		*reg |= (uint16_t)(temp[0] << 5);

		return MAX6675K_OK;
	}
	return MAX6675K_ERROR;
}

float MAX6675K_ReadTemp(uint8_t reg)
{
	return reg * 0.25;
}
