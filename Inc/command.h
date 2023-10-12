#ifndef __COMMAND_H
#define __COMMAND_H

#include "commgr.h"

typedef enum {
	BOOTMODE_SERIAL = 1,
	BOOTMODE_DEVELOPMENT,
	BOOTMODE_FUSE
} BootMode;

#define CMD_PING						((uint16_t)0x01U)
#define CMD_INFO						((uint16_t)0x02U)
#define CMD_PLAY_BUZZER					((uint16_t)0x03U)
#define CMD_PERI_RESET					((uint16_t)0x04U)
#define CMD_CHIP_RESET					((uint16_t)0x05U)
#define CMD_DEVICE_POWER_CTRL			((uint16_t)0x06U)
#define CMD_SWITCH_BOOT_MODE			((uint16_t)0x07U)
#define CMD_RUN_QA						((uint16_t)0x08U)
#define CMD_REPORT_POWER				((uint16_t)0x09U)
#define CMD_WAIT_FOR_TARGET				((uint16_t)0x0AU)
#define CMD_LIST_TP						((uint16_t)0x0BU)
#define CMD_READ_TP						((uint16_t)0x0CU)
#define CMD_SWITCH_AUTO					((uint16_t)0x0DU)

#define __START_CUSTOM_CMD				((uint16_t)0x0BU)

#if defined(STM32)

void CMD_Ping(HID_OutReport *outRp);

void CMD_Info(HID_OutReport *outRp);

void CMD_PlayBuzzer(HID_OutReport *outRp);

void CMD_PeriReset(HID_OutReport *outRp);

void CMD_ChipReset(HID_OutReport *outRp);

void CMD_DevicePowerControl(HID_OutReport *outRp);

void CMD_SwitchBootMode(HID_OutReport *outRp);

void CMD_RunQa(HID_OutReport *outRp);

void CMD_ReportPower(HID_OutReport *outRp);

void CMD_ReportPower(HID_OutReport *outRp);

void CMD_WaitForTarget(HID_OutReport *outRp);
void CMD_WaitForTarget_Cb();

void CMD_ListTP(HID_OutReport *outRp);

void CMD_ReadTP(HID_OutReport *outRp);

void CMD_SwitchToAuto(HID_OutReport *outRp);

#endif // STM32

#endif // __COMMAND_H
