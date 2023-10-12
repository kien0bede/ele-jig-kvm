#include <memory.h>
#include "button_engine.h"
#include "config.h"

#define BTN_FLAG_DOWN	(0x1U)
#define BTN_FLAG_UP		(0x1U << 1)

struct ButtonContext {
	volatile uint32_t flags;
	uint32_t startTime;
	bool activated[BTN_TRIGGER_NUM];
	BtnTriggerCallback cb[BTN_TRIGGER_NUM];
};

static struct ButtonContext g_btnContexts[BTN_MAX_NUM];

void BTN_Init()
{
	memset(g_btnContexts, 0, sizeof(g_btnContexts));
}

void BTN_Tick()
{
	for (uint8_t btnId = 0; btnId < BTN_MAX_NUM; ++btnId)
	{
		struct ButtonContext *ctx = &g_btnContexts[btnId];
		uint32_t flags = ctx->flags;
		bool longPressDetected = false;

		// TODO can be improved by calculating deadline time in tick count
		if (ctx->startTime > 0) {
			uint16_t ms = TICK_TO_MS(HAL_GetTick() - ctx->startTime);
			if (ms >= CONFIG_BUTTON_LONG_PRESS_MS)
			{
				longPressDetected = true;
			}
		}

		if (flags & BTN_FLAG_DOWN)
		{
			if (ctx->activated[BTN_TRIGGER_DOWN])
			{
				ctx->cb[BTN_TRIGGER_DOWN](BTN_TRIGGER_DOWN);
			}

			if (ctx->activated[BTN_TRIGGER_PRESSED] || ctx->activated[BTN_TRIGGER_LONG_PRESSED])
			{
				ctx->startTime = HAL_GetTick();
			}

			ctx->flags &= ~BTN_FLAG_DOWN;
		}
		else if (flags & BTN_FLAG_UP)
		{
			if (ctx->activated[BTN_TRIGGER_UP])
			{
				ctx->cb[BTN_TRIGGER_UP](BTN_TRIGGER_UP);
			}

			if (ctx->activated[BTN_TRIGGER_PRESSED] && !longPressDetected)
			{
				/* If long press callback is already called (ctx->startTime is reset),
				 * we won't call this callback when user released the button */
				if (ctx->startTime > 0) {
					ctx->cb[BTN_TRIGGER_PRESSED](BTN_TRIGGER_PRESSED);
				}
			}

			ctx->startTime = 0;
			ctx->flags &= ~BTN_FLAG_UP;
		}
		else
		{
			if (ctx->activated[BTN_TRIGGER_LONG_PRESSED] && longPressDetected)
			{
				ctx->cb[BTN_TRIGGER_LONG_PRESSED](BTN_TRIGGER_LONG_PRESSED);
				ctx->startTime = 0;
			}
		}
	}
}

void BTN_IRQ_Handler(uint8_t btnId, bool down)
{
	if (down)
	{
		g_btnContexts[btnId].flags |= BTN_FLAG_DOWN;
	}
	else
	{
		g_btnContexts[btnId].flags |= BTN_FLAG_UP;
	}
}

void BTN_RegisterCallback(uint8_t btnId, BtnTriggerType type, BtnTriggerCallback cb)
{
	if (btnId >= BTN_MAX_NUM)
	{
		return;
	}

	g_btnContexts[btnId].activated[type] = true;
	g_btnContexts[btnId].cb[type] = cb;
}

void BTN_UnRegisterCallback(uint8_t btnId, BtnTriggerType type)
{
	if (btnId >= BTN_MAX_NUM)
	{
		return;
	}

	g_btnContexts[btnId].activated[type] = false;
}
