#ifndef __I2C_COMPAT_H
#define __I2C_COMPAT_H

#include <global.h>
#include <stdint.h>
#include <stdbool.h>

extern bool I2C_MemRead(uint8_t devAddr, uint8_t memAddr, uint8_t *data, uint16_t size);

extern bool I2C_MemWrite(uint8_t devAddr, uint8_t memAddr, const uint8_t *data, uint16_t size);

#endif
