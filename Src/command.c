#include "command.h"
#include "controller_board.h"
#include "adapter_board.h"
#include "power_mgr.h"
#include "buzzer_engine.h"

#define MAX_BUZZER_INTERVAL		16000
#define MAX_BUZZER_BREAKTIME	16000
#define MAX_BUZZER_COUNT		15

static uint32_t g_waitForTargetToken;
static bool g_waitFortarget = false;

static void Finish(HID_InReport *inRp, const HID_OutReport *outRp)
{
	inRp->id = HID_IN_REPORT_ID;
	inRp->flags |= HID_IN_FLAG_DONE;
	inRp->token = outRp->token;

	COM_Report(inRp);
}

void CMD_Ping(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	Finish(&inRp, outRp);
}

void CMD_Info(HID_OutReport *outRp)
{
	AdapterBoard *adapter = GetAdapterBoard();
	HID_InReport inRp = {0};

	COM_Trace(adapter->boardName);
	COM_TraceFloat(PM_GetVDDA(), "VDDA=%s", "%.3f");

	Finish(&inRp, outRp);
}

void CMD_PlayBuzzer(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	BuzzerScript script;

	script.interval = outRp->data & 0x3FFF;
	script.breakTime = (outRp->data >> 14) & 0x3FFF;
	script.count = (outRp->data >> 28) & 0xF;

	if (script.interval > MAX_BUZZER_INTERVAL)
	{
		script.interval = MAX_BUZZER_INTERVAL;
	}

	if (script.breakTime > MAX_BUZZER_BREAKTIME)
	{
		script.breakTime = MAX_BUZZER_BREAKTIME;
	}

	if (script.count > MAX_BUZZER_COUNT)
	{
		script.count = MAX_BUZZER_COUNT;
	}

	if (script.interval != 0)
	{
		BZ_Play(script);
	}
	else
	{
		BZ_Stop();
	}

	Finish(&inRp, outRp);
}

void CMD_PeriReset(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};

	PM_ResetPeri();

	Finish(&inRp, outRp);
}

void CMD_ChipReset(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};

	PM_SetChipPOR(false);
	HAL_Delay(100);
	PM_SetChipPOR(true);

	Finish(&inRp, outRp);
}

void CMD_DevicePowerControl(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	PowerStatus stat;

	if (outRp->data <= 1)
	{
		stat = PM_SetDevicePower(outRp->data == 1);
		if (stat != POWER_OK)
		{
			inRp.flags |= HID_IN_FLAG_ERROR;
			inRp.inum = ERROR_CODE_OPERATION_FAILED;
		}
	}
	else
	{
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_INVALID_ARG;
	}

	Finish(&inRp, outRp);
}

void CMD_SwitchBootMode(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	AdapterBoard *adapter = GetAdapterBoard();

	uint8_t mode = outRp->data;

	if (mode > BOOTMODE_FUSE)
	{
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_INVALID_ARG;
	}
	else
	{
		if (!adapter->SwitchBootMode(mode))
		{
			inRp.flags |= HID_IN_FLAG_ERROR;
			inRp.inum = ERROR_CODE_INVALID_ARG;
		}
	}

	Finish(&inRp, outRp);
}

void CMD_RunQa(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};

	QaStatus stat = CB_QaCheck();
	if (stat != QA_PASSED)
	{
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_OPERATION_FAILED;
	}

	Finish(&inRp, outRp);
}

void CMD_ReportPower(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	uint16_t vbus;
	uint16_t vshut;
	uint16_t current;
	uint16_t power;

	if (PM_ReportPower(&vbus, &vshut, &current, &power))
	{
		inRp.flags |= HID_IN_FLAG_DONE;
		uint16_t data[4] = { vbus, vshut, current, power };
		memcpy(inRp.data, data, sizeof(data));
	}
	else
	{
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_OPERATION_FAILED;
	}

	Finish(&inRp, outRp);
}

void CMD_WaitForTarget(HID_OutReport *outRp)
{
	if (CB_IsDeviceConnected())
	{
		HID_InReport inRp = {0};
		Finish(&inRp, outRp);
	}
	else
	{
		g_waitFortarget = true;
		g_waitForTargetToken = outRp->token;
	}
}

void CMD_WaitForTarget_Cb()
{
	HID_InReport inRp = {0};

	if (g_waitFortarget) {
		inRp.id = HID_IN_REPORT_ID;
		inRp.flags |= HID_IN_FLAG_DONE;
		inRp.token = g_waitForTargetToken;

		COM_Report(&inRp);
		g_waitFortarget = false;
	}
}

void CMD_ListTP(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	AdapterBoard *adapter = GetAdapterBoard();

	for (uint8_t i = 0; i < adapter->testPointSpecsSz; ++i)
	{
		TestPointSpec *spec = &adapter->testPointSpecs[i];

		if (spec->activated)
		{
			COM_Trace("%s - %d", spec->name, spec->id);
		}
	}

	Finish(&inRp, outRp);
}

void CMD_ReadTP(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};
	AdapterBoard *adapter = GetAdapterBoard();
	float val;
	bool success = false;
	uint8_t id = outRp->data;

	if (!CB_IsDeviceConnected()) {
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_INVALID_STATE;
		Finish(&inRp, outRp);
		return;
	}

	if (!PM_IsDevicePowered()) {
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_INVALID_STATE;
		Finish(&inRp, outRp);
		return;
	}

	for (uint8_t i = 0; i < adapter->testPointSpecsSz; ++i)
	{
		TestPointSpec *spec = &adapter->testPointSpecs[i];
		if (spec->activated && id == spec->id && PM_ReadVoltage(spec, &val, false))
		{
			success = true;
			break;
		}
	}

	if (success)
	{
		inRp.flags |= HID_IN_FLAG_FLOAT;
		inRp.fnum = val;
	}
	else
	{
		inRp.flags |= HID_IN_FLAG_ERROR;
		inRp.inum = ERROR_CODE_INVALID_ARG;
	}

	Finish(&inRp, outRp);
}

void CMD_SwitchToAuto(HID_OutReport *outRp)
{
	HID_InReport inRp = {0};

	CB_SwitchOperationMode(OPMODE_AUTO);

	Finish(&inRp, outRp);
}
