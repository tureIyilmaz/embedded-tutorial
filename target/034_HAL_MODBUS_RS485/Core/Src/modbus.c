/*
 * modbus.c
 *
 *  Created on: Aug 8, 2023
 *      Author: turel
 */

#include "modbus.h"


/***********************Function Prototips**************************/
static void modbusRtuCRC(const unsigned char Data, unsigned int* CRC);
static unsigned char sendMessage(void);
static void TxRTU(void);
static void RxRTU(void);
static char DoSlaveTx(void);
void HandleModbusError(char ErrorCode);

/****Slave Transmit and Receive Variables****/
unsigned int	Tx_Current	= 0;
unsigned int    Tx_CRC16	= 0xFFFF;
RxTx_State    	Tx_State	= RxTx_IDLE;
unsigned char   Tx_Buf[BUFFER_SIZE];
unsigned int    Tx_Buf_Size	= 0;

RxTx_DATA     	Rx_Data;
unsigned int  	Rx_CRC16	= 0xFFFF;
RxTx_State    	Rx_State	= RxTx_IDLE;


#if READ_HOLDING_REGISTERS_ENABLED > 0
void ModbusReadHoldingRegisters(void)
{
	unsigned int startAddress = 0;
	unsigned int NumberOfRegisters = 0;
	unsigned char   i = 0;


	startAddress		= ((unsigned int) (Rx_Data.dataBuff[0]) << 8) + (unsigned int) (Rx_Data.dataBuff[1]);
	NumberOfRegisters	= ((unsigned int) (Rx_Data.dataBuff[2]) << 8) + (unsigned int) (Rx_Data.dataBuff[3]);


	Tx_Data.modbusFunction   	= ReadHoldingRegisters;
	Tx_Data.modbusSlaveAddress	= Device1SlaveAddress;
	Tx_Data.dataLen     		= 4;

	if(StartAddress + NumberOfRegisters)>NUMBER_OF_OUTPUT_REGISTERS)
	{
		HandleModbusError(ErrorCode2);
	}
	else
	{
		for (i = 0; i < NumberOfRegisters; i++)
		{
			unsigned short currentData = Registers[startAddress + i].ActValue;

			Tx_Data.dataBuff[Tx_Data.dataLen]        = (unsigned char) ((currentData & 0xFF00) >> 8);
			Tx_Data.dataBuff[Tx_Data.dataLen + 1]    = (unsigned char) (currentData & 0xFF);
			Tx_Data.dataLen                        += 2;
			Tx_Data.dataBuff[0]	 = Rx_Data.dataLen - 1;
		}
	}


	sendMessage();
}
#endif


#if WRITE_SINGLE_REGISTER_ENABLED > 0
void ModbusPresetSingleRegister(void)
{
	unsigned int startAddress = 0;
	unsigned int val = 0;
	unsigned char   i = 0;


	startAddress		= ((unsigned int) (Rx_Data.dataBuff[0]) << 8) + (unsigned int) (Rx_Data.dataBuff[1]);
	NumberOfRegisters	= ((unsigned int) (Rx_Data.dataBuff[2]) << 8) + (unsigned int) (Rx_Data.dataBuff[3]);


	Tx_Data.modbusFunction   	= PresetSingleRegister;
	Tx_Data.modbusSlaveAddress	= Device1SlaveAddress;
	Tx_Data.dataLen     		= 4;

	if(Address>=NUMBER_OF_OUTPUT_EGISTERS)
	{
		HandleModbusError(ErrorCode2);
	}

	    else
	    {
	        Registers[Address].ActValue=Val;

	        for (i = 0; i < 4; i)
	        {
	        	Tx_Data.DataBuff[i] = Rx_Data.DataBuff[i];
	        }
	    }

	    sendMessage();

}
#endif


void ProcessModbus(void)
{
    if (Tx_State != RxTx_IDLE)
        TxRTU();

    RxRTU();

    if (RxDataAvailable())
    {
        if (Rx_Data.modbusSlaveAddress == Device1SlaveAddress)
        {
            switch (Rx_Data.modbusFunction)
            {
                #if MODBUS_READ_HOLDING_REGISTERS_ENABLED > 0
                case MODBUS_READ_HOLDING_REGISTERS:
                {
                	HandleModbusReadHoldingRegisters();
                	break;
                }
                #endif
                #if MODBUSWRITE_SINGLE_REGISTER_ENABLED > 0
                case MODBUS_WRITE_SINGLE_REGISTER:
                {
                	HandleModbusWriteSingleRegister();
                	break;
                }
                #endif
                default:
                {
                	HandleModbusError(ErrorCode1);
                	break;
                }
            }
        }
    }
}

static char DoSlaveTx(void)
{
	ModBus_UART_String(Tx_Buf,Tx_Buf_Size);
	Tx_Buf_Size = 0;
	return 1;
}


static void RxRTU(void)
{
    unsigned char   i;
    unsigned char   ReceiveBufferControl = 0;

    ReceiveBufferControl    =CheckModbusBufferComplete();

    if(ReceiveBufferControl==DATA_READY)
    {
        Rx_Data.modbusSlaveAddress = ReceiveBuffer[0];
        Rx_CRC16 = 0xFFFF;
        modbusRtuCRC(Rx_Data.modbusSlaveAddress, &Rx_CRC16);
        Rx_Data.modbusFunction	= ReceiveBuffer[1];
        modbusRtuCRC(Rx_Data.modbusFunction, &Rx_CRC16);

        Rx_Data.dataLen=0;

        for(i = 2; i < ReceiveCounter ; i++)
        {
        	Rx_Data.dataBuff[Rx_Data.dataLen++]=ReceiveBuffer[i];
        }


        Rx_State =RxTx_DATABUF;

        ReceiveCounter = 0;
    }
}


static void TxRTU(void)
{
    Tx_CRC16                =0xFFFF;
    Tx_Buf_Size             =0;
    Tx_Buf[Tx_Buf_Size++]   = Tx_Data.modbusSlaveAddress;
    modbusRtuCRC(Tx_Data.modbusSlaveAddress, &Tx_CRC16);
    Tx_Buf[Tx_Buf_Size++]   =Tx_Data.modbusFunction;
    modbusRtuCRC(Tx_Data.modbusFunction, &Tx_CRC16);

    for(Tx_Current=0; Tx_Current < Tx_Data.dataLen; Tx_Current++)
    {
        Tx_Buf[Tx_Buf_Size++]=Tx_Data.dataBuff[Tx_Current];
        modbusRtuCRC(Tx_Data.dataBuff[Tx_Current], &Tx_CRC16);
    }

    Tx_Buf[Tx_Buf_Size++] = Tx_CRC16 & 0x00FF;
    Tx_Buf[Tx_Buf_Size++] =(Tx_CRC16 & 0xFF00) >> 8;

    DoSlaveTx();

    Tx_State = RxTx_IDLE;
}

void HandleModbusError(char ErrorCode)
{

    Tx_Data.modbusFunction    = Rx_Data.modbusFunction | 0x80;
    Tx_Data.modbusSlaveAddress     = Device1SlaveAddress;
    Tx_Data.dataLen     = 1;
    Tx_Data.dataBuff[0]  = ErrorCode;
    sendMessage();
}



static unsigned char sendMessage(void)
{
    if (Tx_State != RxTx_IDLE)
        return 0;

    Tx_Current  = 0;
    Tx_State    = RxTx_START;

    return 1;
}


static void modbusRtuCRC(const unsigned char Data, unsigned int* CRC)
{
    unsigned int i;

    *CRC = *CRC ^(unsigned int) Data;
    for (i = 8; i > 0; i--)
    {
        if (*CRC & 0x0001)
            *CRC = (*CRC >> 1) ^ 0xA001;
        else
            *CRC >>= 1;
    }
}



/*
static uint16_t modbusRtuCRC(uint8_t *buff, int len)
{

	uint16_t crc = 0xFFFF;

	for (int pos = 0; pos < len; pos++)
	{
		crc ^= *(buff + pos);

	    for (int i = 8; i != 0; i--)
	    {
	    	if ((crc & 0x0001) != 0)
	    	{
	    		crc >>= 1;
	    		crc ^= 0xA001;
	    	}
	    	else
	    	{
	    		crc >>= 1;
	    	}
	    }
	  }
	  return crc;
}
*/

