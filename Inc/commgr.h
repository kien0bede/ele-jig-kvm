#ifndef __COMMGR_H
#define __COMMGR_H

#if defined(STM32)
#include "usbd_customhid.h"
#include <stdbool.h>
#else
#include <stdint.h>
#endif // STM32

#define HID_IN_REPORT_ID		0x1
#define HID_OUT_REPORT_ID		0x2

#define VENDOR_ID				1155
#define PRODUCT_ID				22352

typedef struct {
#ifdef _MSC_VER
	__pragma(pack(push, 1))
#endif
	uint8_t id;
	uint32_t token;
	uint16_t flags;
	union {
		int32_t inum;
		float fnum;
		uint8_t data[32];
	};
	uint8_t reversed;
#ifdef _MSC_VER
	__pragma(pack(pop))
} HID_InReport;
#else
} __attribute__((packed)) HID_InReport;
#endif

static_assert(sizeof(HID_InReport) == 40, "Invalid HID_InReport size");

typedef struct {
#ifdef _MSC_VER
	__pragma(pack(push, 1))
#endif
	uint8_t id;
	uint32_t token;
	uint16_t type;
	uint32_t data;
	uint8_t reversed;
#ifdef _MSC_VER
	__pragma(pack(pop))
} HID_OutReport;
#else
} __attribute__((packed)) HID_OutReport;
#endif

static_assert(sizeof(HID_OutReport) == 12, "Invalid HID_OutReport size");

#define HID_IN_FLAG_TRACE				((uint16_t)0x1U)		    // Input report contains trace message
#define HID_IN_FLAG_DONE				((uint16_t)0x1U << 1)		// Input report indicating request done
#define HID_IN_FLAG_ERROR				((uint16_t)0x1U << 2)		// Input report contains error code
#define HID_IN_FLAG_MSG					((uint16_t)0x1U << 3)		// Input report contains printable message
#define HID_IN_FLAG_KEYVAL				((uint16_t)0x1U << 4)		// Input report contains a key and value
#define HID_IN_FLAG_FLOAT				((uint16_t)0x1U << 5)		// Input report contains a float
#define HID_IN_FLAG_INT					((uint16_t)0x1U << 6)		// Input report contains an integer

#define ERROR_CODE_UNKNOWN_CMD			0x1U
#define ERROR_CODE_INVALID_ARG			0x2U
#define ERROR_CODE_OPERATION_FAILED		0x3U
#define ERROR_CODE_NOT_SUPPORTED		0x4U
#define ERROR_CODE_INVALID_STATE		0x5U

#if defined(STM32)

void COM_Trace(const char *msg, ...);

void COM_TraceFloat(float fnum, const char *msg, ...);

void COM_Report(const HID_InReport *inRp);

void COM_SetEnabled(bool enabled);

#endif // STM32

#endif
