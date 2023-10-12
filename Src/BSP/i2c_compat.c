#include <stdio.h>
#include "i2c_compat.h"
#include "global.h"

__attribute__((weak))
bool I2C_MemRead(uint8_t devAddr, uint8_t memAddr, uint8_t *data, uint16_t size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, devAddr, memAddr, sizeof(memAddr), data, size, 1000);
	if (status != HAL_OK) {
		return false;
	}
	return true;
}

__attribute__((weak))
bool I2C_MemWrite(uint8_t devAddr, uint8_t memAddr, const uint8_t *data, uint16_t size)
{
	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, devAddr, memAddr, sizeof(memAddr), (uint8_t*)data, size, 1000);
	if (status != HAL_OK) {
		return false;
	}
	return true;
}
