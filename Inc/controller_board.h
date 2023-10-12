#ifndef __CONTROLLER_BOARD_H
#define __CONTROLLER_BOARD_H

#include "global.h"
#include "command.h"

#define BTN_ID_USER		0
#define BTN_ID_RESET	1

void CB_Init();

void CB_Yeild();

void CB_IRQ_HID_DataOut(uint8_t *buf);

void CB_IRQ_Handler(IRQn_Type irq);

void CB_IRQ_OnTargetReset(bool asserted);

void CB_OnBootModeChanged(BootMode mode);

OperationMode CB_GetOperationMode();

bool CB_IsDeviceConnected();

void CB_SwitchOperationMode(OperationMode mode);

QaStatus CB_QaCheck();

#endif
