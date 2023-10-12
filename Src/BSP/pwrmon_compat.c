#include "ina230.h"
#include "i2c_compat.h"

uint8_t PWRMON_IO_Init(uint16_t Addr)
{
	return 0;
}

void PWRMON_IO_DeInit(uint16_t Addr)
{

}

uint8_t PWRMON_IO_Write(uint16_t Addr, uint8_t Reg, uint8_t * pData)
{
	I2C_MemWrite(Addr, Reg, pData, sizeof(uint16_t));
	return 0;
}

uint8_t PWRMON_IO_Read(uint16_t Addr, uint8_t Reg, uint8_t * pData)
{
	I2C_MemRead(Addr, Reg, pData, sizeof(uint16_t));
	return 0;
}

void PWRMON_IO_EnableIT(uint16_t Addr, uint8_t ActiveEdge)
{

}

void PWRMON_IO_DisableIT(uint16_t Addr)
{

}
