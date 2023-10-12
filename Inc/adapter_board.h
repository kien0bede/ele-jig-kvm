#ifndef __ADAPTER_BOARD_H
#define __ADAPTER_BOARD_H

#include "global.h"
#include "commgr.h"
#include "power_mgr.h"
#include "command.h"

typedef struct {
	const char *boardName;
	IRQn_Type *irqList;
	uint8_t irqListSz;
	TestPointSpec *testPointSpecs;
	uint8_t testPointSpecsSz;
	PinInfo *gndDetect;

	void (*Init)();
	void (*EnterNewMode)(OperationMode mode);
	bool (*Handle_HID_OutReport)(HID_OutReport *outRp);
	bool (*SwitchBootMode)(BootMode mode);
	void (*IRQ_Handler)(IRQn_Type);
} AdapterBoard;

extern AdapterBoard *GetAdapterBoard(void);

#endif
