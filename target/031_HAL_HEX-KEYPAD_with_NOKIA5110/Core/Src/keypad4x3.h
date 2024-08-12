/*
 * keypad3x4.h
 *
 *  Created on: Jul 24, 2023
 *      Author: turel
 */

#ifndef SRC_KEYPAD4X3_H_
#define SRC_KEYPAD4X3_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"

typedef struct {
    GPIO_TypeDef* IN_Port;
    GPIO_TypeDef* OUT_Port;

    uint16_t IN_PIN_0;
    uint16_t IN_PIN_1;
    uint16_t IN_PIN_2;
    uint16_t IN_PIN_3;
    uint16_t OUT_PIN_0;
    uint16_t OUT_PIN_1;
    uint16_t OUT_PIN_2;

} Keypad4x3_WiresTypeDef;



void Keypad4x3_Init(Keypad4x3_WiresTypeDef *Keypad_WiringStruct);

void Keypad4x3_ReadKeypad(bool keys[12]);




#endif /* SRC_KEYPAD4X3_H_ */
