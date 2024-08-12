/*
 * keypad4x3.c
 *
 *  Created on: Jul 24, 2023
 *      Author: turel
 */

#include "keypad4x3.h"

Keypad4x3_WiresTypeDef KeypadStruct;

uint8_t row, col;
char keymap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void Keypad4x3_Init(Keypad4x3_WiresTypeDef* keypadWires) {
    // Keypad yapısını kaydedin
    KeypadStruct = *keypadWires;

    // Giriş pinlerini tanımlayın
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    GPIO_InitStruct.Pin = KeypadStruct.IN_PIN_0;
    HAL_GPIO_Init(KeypadStruct.IN_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KeypadStruct.IN_PIN_1;
    HAL_GPIO_Init(KeypadStruct.IN_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KeypadStruct.IN_PIN_2;
    HAL_GPIO_Init(KeypadStruct.IN_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KeypadStruct.IN_PIN_3;
    HAL_GPIO_Init(KeypadStruct.IN_Port, &GPIO_InitStruct);

    // Çıkış pinlerini tanımlayın
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = KeypadStruct.OUT_PIN_0;
    HAL_GPIO_Init(KeypadStruct.OUT_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KeypadStruct.OUT_PIN_1;
    HAL_GPIO_Init(KeypadStruct.OUT_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KeypadStruct.OUT_PIN_2;
    HAL_GPIO_Init(KeypadStruct.OUT_Port, &GPIO_InitStruct);
}

void Keypad4x3_ReadKeypad(bool keys[12]) {
    // Keypad'deki tuşları okuyun

    for (col = 0; col < 3; col++) {
        HAL_GPIO_WritePin(KeypadStruct.OUT_Port, KeypadStruct.OUT_PIN_0 << col, GPIO_PIN_RESET);

        // Belirli bir sütundaki tuşları oku
        for (row = 0; row < 4; row++) {
            keys[col * 4 + row] = (HAL_GPIO_ReadPin(KeypadStruct.IN_Port, KeypadStruct.IN_PIN_0 << row) == GPIO_PIN_RESET);
        }

        // Sütunu tekrar pasif yap
        HAL_GPIO_WritePin(KeypadStruct.OUT_Port, KeypadStruct.OUT_PIN_0 << col, GPIO_PIN_SET);
    }
}
