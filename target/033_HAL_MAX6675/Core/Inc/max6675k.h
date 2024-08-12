/*
 * max6675k.h
 *
 *  Created on: Jul 26, 2023
 *      Author: turel
 */

#ifndef INC_MAX6675K_H_
#define INC_MAX6675K_H_

#include "main.h"

#define MAX6675K_OK 1
#define MAX6675K_ERROR 0

extern SPI_HandleTypeDef hspi1;;


uint8_t MAX6675K_ReadRegisters(uint16_t *reg);
float MAX6675K_ReadTemp(uint8_t deviceValue);



#endif /* INC_MAX6675K_H_ */
