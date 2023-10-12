#include <stdarg.h>
#include <stdio.h>
#include "commgr.h"
#include "config.h"
#include "global.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t g_tx_buffer[50];
static bool g_comEnabled = false;

void COM_Trace(const char *msg, ...)
{
	if (!g_comEnabled) {
		return;
	}

	HID_InReport rp = {0};
	va_list args;

	rp.id = HID_IN_REPORT_ID;
	rp.token = 0;
	rp.flags = HID_IN_FLAG_TRACE | HID_IN_FLAG_MSG;

	va_start(args, msg);
	vsnprintf((char*)rp.data, sizeof(rp.data) - 1, msg, args);
	va_end(args);

	COM_Report(&rp);
}

void COM_TraceFloat(float fnum, const char *msg, ...)
{
	if (!g_comEnabled) {
		return;
	}

	HID_InReport rp = {0};
	va_list args;

	rp.id = HID_IN_REPORT_ID;
	rp.token = 0;
	rp.flags = HID_IN_FLAG_TRACE | HID_IN_FLAG_MSG | HID_IN_FLAG_FLOAT;
	rp.fnum = fnum;

	va_start(args, msg);
	vsnprintf((char*)rp.data + sizeof(float), sizeof(rp.data) - sizeof(float) - 1, msg, args);
	va_end(args);

	COM_Report(&rp);
}

void COM_Report(const HID_InReport *inRp)
{
	if (!g_comEnabled) {
		return;
	}

	USBD_StatusTypeDef stat;
	uint32_t deadline = HAL_GetTick() + MS_TO_TICK(CONFIG_SEND_REPORT_TIMEOUT);

	memcpy(g_tx_buffer, inRp, sizeof(HID_InReport));

	do {
		stat = USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, g_tx_buffer, sizeof(HID_InReport));
	} while (stat == USBD_BUSY && HAL_GetTick() < deadline);
}

void COM_SetEnabled(bool enabled)
{
	g_comEnabled = enabled;
}
