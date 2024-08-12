/*
 * nokia5110.h
 *
 *  Created on: Jul 20, 2023
 *      Author: turel
 */

#ifndef SRC_NOKIA5110_H_
#define SRC_NOKIA5110_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "string.h"
#include "ctype.h"

extern SPI_HandleTypeDef hspi2;
extern DMA_HandleTypeDef hdma_spi2_tx;

extern uint8_t frameBuff[504];

bool nokia5110_Init(void);
bool nokia5110_Update(void);
void nokia5110_Reset(void);
void nokia5110_SetPixel(uint8_t x, uint8_t y, bool set);
void nokia5110_Clear(void);
bool nokia5110_Write(uint8_t data, uint8_t mode);
void nokia_bufferWrite(uint8_t* data, uint16_t length);
void nokia5110_WriteCharacter(char word[]);

#endif /* SRC_NOKIA5110_H_ */
