#ifndef __BUZZER_ENGINE_H
#define __BUZZER_ENGINE_H

#include "global.h"

typedef struct {
	uint16_t interval;
	uint16_t breakTime;
	uint8_t count;
} BuzzerScript;

#define BUZZER_SCRIPT_ONESHOT(inl) \
	{ .interval = inl, .breakTime = 0, .count = 1 }

void BZ_Init();

void BZ_IRQ_SysTick_Handler();

void BZ_Play(BuzzerScript script);

void BZ_Stop();

#endif
