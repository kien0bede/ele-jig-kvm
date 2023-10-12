#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "stm32f0xx_hal.h"
#include "main.h"

#define INVALID_IRQ		(-99)

#define ADC_PIN_NUM		10

#define ADC_INPUT_0		{ .port = GPIOA, .pin = GPIO_PIN_0 }
#define ADC_INPUT_1		{ .port = GPIOA, .pin = GPIO_PIN_1 }
#define ADC_INPUT_2		{ .port = GPIOA, .pin = GPIO_PIN_2 }
#define ADC_INPUT_3		{ .port = GPIOA, .pin = GPIO_PIN_3 }
#define ADC_INPUT_4		{ .port = GPIOA, .pin = GPIO_PIN_4 }
#define ADC_INPUT_5		{ .port = GPIOA, .pin = GPIO_PIN_5 }
#define ADC_INPUT_6		{ .port = GPIOA, .pin = GPIO_PIN_6 }
#define ADC_INPUT_7		{ .port = GPIOA, .pin = GPIO_PIN_7 }
#define ADC_INPUT_8		{ .port = GPIOB, .pin = GPIO_PIN_0 }
#define ADC_INPUT_9		{ .port = GPIOB, .pin = GPIO_PIN_1 }

#define TICK_TO_MS(t) 	((t) * (uint32_t)HAL_GetTickFreq())
#define MS_TO_TICK(ms) 	((ms) / (uint32_t)HAL_GetTickFreq())

extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc;

typedef struct {
	uint16_t pin;
	GPIO_TypeDef *port;
} PinInfo;

typedef enum {
	OPMODE_INVALID = 0,
	OPMODE_AUTO,
	OPMODE_CONTROL,
	OPMODE_NUM
} OperationMode;

typedef enum {
	QA_PASSED = 0,
	QA_VOLTAGES_FAILED_BUT_ACCEPTED,
	QA_DEVICE_NOT_FOUND,
	QA_DEVICE_NOT_POWERED,
	QA_VOLTAGES_FAILED,
	QA_POWER_FAILED,
} QaStatus;

#endif
