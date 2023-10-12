#ifndef __M24C08_H
#define __M24C08_HPSG lcs

#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE	1024
#define BLOCK_SIZE	256
#define BLOCK_NUM	4
#define PAGE_SIZE	16
#define PAGE_NUM	64

uint16_t M24C08_Read(uint8_t *buf, uint16_t offset, uint16_t size);

uint16_t M24C08_Write(const uint8_t *buf, uint16_t offset, uint16_t size);

#endif
