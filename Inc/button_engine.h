#ifndef __BUTTON_ENGINE_H
#define __BUTTON_ENGINE_H

#include "global.h"

/* Only 2 button existing on the board currently */
#define BTN_MAX_NUM		2

typedef enum {
	BTN_TRIGGER_DOWN = 0,
	BTN_TRIGGER_UP,
	BTN_TRIGGER_PRESSED,
	BTN_TRIGGER_LONG_PRESSED,
	BTN_TRIGGER_NUM
} BtnTriggerType;

typedef void (*BtnTriggerCallback)(BtnTriggerType type);

void BTN_Init();

void BTN_Tick();

void BTN_IRQ_Handler(uint8_t btnId, bool down);

void BTN_RegisterCallback(uint8_t btnId, BtnTriggerType type, BtnTriggerCallback cb);

void BTN_UnRegisterCallback(uint8_t btnId, BtnTriggerType type);

#endif
