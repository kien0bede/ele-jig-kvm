#include "m24c08.h"
#include "i2c_compat.h"

#define DEVICE_ADDRESS	((uint8_t)0xA0U)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

uint16_t M24C08_Read(uint8_t *buf, uint16_t offset, uint16_t size)
{
	if (offset >= MEM_SIZE) {
		return 0;
	}

	if ((int)offset + size > MEM_SIZE) {
		size = MEM_SIZE - offset;
	}

	uint16_t nr = 0;
	uint8_t block = offset / BLOCK_SIZE;
	uint16_t boff = offset % BLOCK_SIZE;
	uint16_t rdlen;
	uint8_t addr;

	while (nr < size) {
		rdlen = MIN(BLOCK_SIZE - boff, size - nr);
		addr = DEVICE_ADDRESS | (block << 1);

		if (I2C_MemRead(addr, boff, buf + nr, rdlen) == true) {
			block++;
			boff = 0;
			nr += rdlen;
		} else {
			break;
		}
	}

	return nr;
}

static uint16_t M24C08_WriteBlock(uint8_t block, const uint8_t *buf, uint16_t boff, uint16_t size)
{
	if ((int)boff + size > BLOCK_SIZE) {
		size = BLOCK_SIZE - boff;
	}

	uint8_t addr = DEVICE_ADDRESS | (block << 1);
	uint16_t nw = 0;
	uint16_t wrlen;

	while (nw < size) {
		uint16_t poff = boff % PAGE_SIZE;
		if (poff == 0) {
			wrlen = MIN(PAGE_SIZE, size - nw);
		} else {
			wrlen = MIN(PAGE_SIZE - poff, size - nw);
		}
		if (I2C_MemWrite(addr, boff, buf + nw, wrlen)) {
			nw += wrlen;
			boff += wrlen;
		} else {
			break;
		}
#if !defined(UNIT_TEST)
		HAL_Delay(6);
#endif
	}

	return nw;
}

uint16_t M24C08_Write(const uint8_t *buf, uint16_t offset, uint16_t size)
{
	if (offset >= MEM_SIZE) {
		return 0;
	}

	if ((int)offset + size > MEM_SIZE) {
		size = MEM_SIZE - offset;
	}

	uint16_t nw = 0;
	uint8_t block = offset / BLOCK_SIZE;
	uint16_t boff = offset % BLOCK_SIZE;

	while (nw < size) {
		uint16_t n = M24C08_WriteBlock(block, buf + nw, boff, size - nw);
		if (n == 0) {
			break;
		}
		if (boff + n == BLOCK_SIZE) {
			block++;
			boff = 0;
		} else {
			boff += n;
		}
		nw += n;
	}

	return nw;
}
