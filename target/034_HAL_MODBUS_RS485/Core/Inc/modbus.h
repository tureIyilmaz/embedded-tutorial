/*
 * modebus.h
 *
 *  Created on: Aug 8, 2023
 *      Author: turel
 */

#ifndef INC_MODBUS_H_
#define INC_MODBUS_H_


#include <stdint.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024


typedef enum{
    ReadHoldingRegisters = 0x03,
    PresetSingleRegister = 0x06,
//    PresetMultipleRegisters = 0x10
}ModbusFunction;


typedef enum{
	Device1SlaveAddress = 0x01,
	Device2SlaveAddress = 0x02,
}ModbusSlaveAddress;


typedef enum
{
    RxTx_IDLE = 0,
    RxTx_START,
    RxTx_DATABUF,
    RxTx_WAIT_ANSWER,
    RxTx_TIMEOUT
}RxTx_State;


//DUZELTİLECEK
typedef enum
{
	FALSE_FUNCTION = 0,
	FALSE_SLAVE_ADDRESS,
	DATA_NOT_READY,
	DATA_READY
}State;


typedef enum
{
	ErrorCode1 = 1,
	ErrorCode2 = 2
}ErrorCode;


typedef struct{
	ModbusSlaveAddress modbusSlaveAddress;
	ModbusFunction  modbusFunction;
	unsigned char  dataBuff[BUFFER_SIZE];
	unsigned short dataLen;
}RxTx_DATA;


// ModbusSlaveAddress slaveAddress, uint16_t startAddress, uint16_t numRegisters
void ModbusReadHoldingRegisters(void);
// ModbusSlaveAddress slaveAddress, uint16_t registerAddress, uint16_t value
void ModbusPresetSingleRegister(void);
//ModbusSlaveAddress slaveAddress, uint16_t startAddress, uint16_t *values, uint16_t numRegisters
//void ModbusPresetMultipleRegs(void);


#endif /* INC_MODBUS_H_ */
