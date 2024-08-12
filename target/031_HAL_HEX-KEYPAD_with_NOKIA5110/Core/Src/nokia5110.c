/*
 * nokia5510.c
 *
 *  Created on: Jul 20, 2023
 *      Author: turel
 */


#include "nokia5110.h"



bool nokia5110_Init(void)
{
	nokia5110_Reset();

	if(!(nokia5110_Write(0x21, 0)))
	{
		return false;
	}
	if(!(nokia5110_Write(0xC0, 0)))
	{
		return false;
	}
	if(!(nokia5110_Write(0x07, 0)))
	{
		return false;
	}
	if(!(nokia5110_Write(0x13, 0)))
	{
		return false;
	}
	if(!(nokia5110_Write(0x20, 0)))
	{
		return false;
	}
	if(!(nokia5110_Write(0x0C, 0)))
	{
		return false;
	}

	return true;
}

bool nokia5110_Update(void)
{

	if(!(nokia5110_Write(0x80, 0))) // move to x = 0
	{
		return false;
	}

	if(!(nokia5110_Write(0x40, 0))) // move to y = 0
	{
		return false;
	}

	nokia_bufferWrite(frameBuff, 504);

	return true;
}


void nokia5110_Reset(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET); // reset is low
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET); // reset is high
}


void nokia5110_SetPixel(uint8_t x, uint8_t y, bool set)
{
	uint8_t Bi;
	uint16_t By;

	if(x  < 0 || x >= 84 || y < 0 || y >= 84 )
		return;


	By = (y / 8) * 84 + x;
	Bi = y % 8;

	if(set)
	{
		frameBuff[By] |= (1 << Bi);
	}
	else
	{
		frameBuff[By] &= ~(1 << Bi);
	}
}


void nokia5110_Clear(void)
{
	for(int i = 0; i < 504; i++)
	{
		frameBuff[i] = 0x00;
	}
}




bool nokia5110_Write(uint8_t data, uint8_t mode)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET); // CE is low

	if( mode == 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET); // DC is low
	}
	else if( mode == 1)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET); // DC is high
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET); // CE is high
		return false;
	}

	HAL_SPI_Transmit_DMA(&hspi2, &data, 1);


	return true;
}


void nokia_bufferWrite(uint8_t* data, uint16_t length)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET); // CE is low
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET); // DC is high
	HAL_SPI_Transmit_DMA(&hspi2, data, length);

}


void nokia5110_WriteString(char word[])
{
    uint8_t x = 0;
    uint8_t y = 0;

	for(int i = 0; i< strlen(word); i++)
	{
		char character = word[i];
		character = toupper(character);

	switch(character)
	{
	case 'A':
	{
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;

	}
	case 'B':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;

	}
	case 'C':
	{
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}

	case 'D':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;
	}
	case 'E':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}
	case 'F':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		break;
	}
	case 'G':
	{
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}
	case 'H':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}
	case 'I':
	{
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 1, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 3, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    break;
	}
	case 'J':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;
	}
	case 'K':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}
	case 'L':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}
	case 'M':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 6, y + 0, 1);
		nokia5110_SetPixel(x + 6, y + 1, 1);
		nokia5110_SetPixel(x + 6, y + 2, 1);
		nokia5110_SetPixel(x + 6, y + 3, 1);
		nokia5110_SetPixel(x + 6, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 1, 1);
		nokia5110_SetPixel(x + 5, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 4, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		break;
	}
	case 'N':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 4, y + 0, 1);
		nokia5110_SetPixel(x + 4, y + 1, 1);
		nokia5110_SetPixel(x + 4, y + 2, 1);
		nokia5110_SetPixel(x + 4, y + 3, 1);
		nokia5110_SetPixel(x + 4, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		break;
	}
	case 'O':
	{
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 4, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 4, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 4, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		break;
	}
	case 'P':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		break;
	}
	case 'Q':
	{
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 4, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 4, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		nokia5110_SetPixel(x + 4, y + 4, 1);
		break;
	}
	case 'R':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 4, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 4, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 3, 1);
		nokia5110_SetPixel(x + 2, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);

		break;
	}
	case 'S':
	{
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;
	}
	case 'T':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 4, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 3, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;
	}
	case 'U':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;
	}
	case 'V':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 3, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 5, 1);
		nokia5110_SetPixel(x + 2, y + 5, 1);
		break;
	}
	case 'W':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 4, y + 0, 1);
		nokia5110_SetPixel(x + 0, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 1, 1);
		nokia5110_SetPixel(x + 4, y + 1, 1);
		nokia5110_SetPixel(x + 0, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 4, y + 2, 1);
		nokia5110_SetPixel(x + 0, y + 3, 1);
		nokia5110_SetPixel(x + 2, y + 3, 1);
		nokia5110_SetPixel(x + 4, y + 3, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);

		break;
	}
	case 'X':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 4, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 3, 1);
		nokia5110_SetPixel(x + 3, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 4, y + 4, 1);
		break;
	}
	case 'Y':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 4, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 1, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 2, y + 3, 1);
		nokia5110_SetPixel(x + 2, y + 4, 1);
		break;
	}
	case 'Z':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
		nokia5110_SetPixel(x + 1, y + 0, 1);
		nokia5110_SetPixel(x + 2, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 0, 1);
		nokia5110_SetPixel(x + 4, y + 0, 1);
		nokia5110_SetPixel(x + 3, y + 1, 1);
		nokia5110_SetPixel(x + 2, y + 2, 1);
		nokia5110_SetPixel(x + 1, y + 3, 1);
		nokia5110_SetPixel(x + 0, y + 4, 1);
		nokia5110_SetPixel(x + 1, y + 4, 1);
		nokia5110_SetPixel(x + 3, y + 4, 1);
		nokia5110_SetPixel(x + 4, y + 4, 1);
		break;
	}
	case '0':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '1':
	{
		nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 1, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 3, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '2':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '3':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '4':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '5':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '6':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '7':
	{
		nokia5110_SetPixel(x + 0, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 0, y + 4, 1);
	    break;
	}
	case '8':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '9':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 2, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 2, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);
	    break;
	}
	case '*':
	{
	    nokia5110_SetPixel(x + 0, y + 0, 1);
	    nokia5110_SetPixel(x + 4, y + 0, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 2, y + 1, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 0, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 2, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 4, y + 2, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 2, y + 3, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 0, y + 4, 1);
	    nokia5110_SetPixel(x + 4, y + 4, 1);
	    break;
	}
	case '#':
	{
	    nokia5110_SetPixel(x + 1, y + 0, 1);
	    nokia5110_SetPixel(x + 3, y + 0, 1);
	    nokia5110_SetPixel(x + 0, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 1, 1);
	    nokia5110_SetPixel(x + 2, y + 1, 1);
	    nokia5110_SetPixel(x + 3, y + 1, 1);
	    nokia5110_SetPixel(x + 4, y + 1, 1);
	    nokia5110_SetPixel(x + 1, y + 2, 1);
	    nokia5110_SetPixel(x + 3, y + 2, 1);
	    nokia5110_SetPixel(x + 0, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 3, 1);
	    nokia5110_SetPixel(x + 2, y + 3, 1);
	    nokia5110_SetPixel(x + 3, y + 3, 1);
	    nokia5110_SetPixel(x + 4, y + 3, 1);
	    nokia5110_SetPixel(x + 1, y + 4, 1);
	    nokia5110_SetPixel(x + 3, y + 4, 1);

	    break;
	}

	default:
	{
		  break;
	}
	}

	x += 6;

    if (x >= 80)
    {
        x = 0;
        y += 6;
    }

    if (y >= 48)
    {

        break;
    }

	}
}



