#ifndef __POWER_MGR_H
#define __POWER_MGR_H

#include "global.h"

#define TEST_POINT_UNUSED() { .activated = false }

/*
 * @brief Define test point specification structure
 *
 * @param ID 		id number
 * @param NAME 		pin name
 * @param ADC_PIN 	ADC pin in type PinInfo, see global.h
 * @param ADC_CHL 	ADC channel of ADC_PIN
 * @param CS_EN 	enable check short en for this pin
 * @param CS_VOL 	vdd connected to this pin when check short enabled
 * @param CS_TOL 	vdd tolerance, the measured voltage must be greater CS_TOL*CS_VSYS,
 * 		   				otherwise this rail is considered to be shorted
 * @param VOL		expected voltage of this rail
 * @param TOL 		tolerance of VOL
 * @param CALIB		linear calib value (in V)
 */
#define TEST_POINT_SPEC(ID, NAME, ADC_PIN, ADC_CHL, HALF, CS_EN, CS_VOL, CS_TOL, VOL, TOL, CALIB) \
	{ \
		.id = ID, \
		.name = NAME, \
		.activated = true, \
		.adcPin = ADC_PIN, \
		.adcChannel = ADC_CHL, \
		.isHalf = HALF, \
		.failureAccepted = false, \
		.chkShortActivated = CS_EN, \
		.chkShortVol = CS_VOL, \
		.chkShortVolTolPercent = CS_TOL, \
		.testPointVol = VOL, \
		.testPointVolTol = TOL, \
		.testPointVolLinearCalib = CALIB \
	}

typedef enum {
	POWER_OK,
	POWER_DEVICE_NOT_ATTACHED,
	POWER_SHORT_DETECTED
} PowerStatus;

typedef struct {
	uint8_t id;
	const char *name;
	bool activated;
	PinInfo adcPin;
	uint32_t adcChannel;
	bool isHalf;
	bool failureAccepted; // TODO: Workaround for VSYS 3V8 Mobiz LTE
	bool chkShortActivated;
	float chkShortVol;
	float chkShortVolTolPercent;
	float testPointVol;
	float testPointVolTol;
	float testPointVolLinearCalib;
} TestPointSpec;

void PM_Init();

void PM_ResetPeri();

void PM_SetChipPOR(bool on);

PowerStatus PM_SetDevicePower(bool on);

void PM_VerifyVoltages(bool *passed, bool *accepted);

bool PM_IsDevicePowered();

void PM_SetVDDA(float vdda);

float PM_GetVDDA();

bool PM_ReadVoltage(const TestPointSpec *spec, float *val, bool raw);

bool PM_ReportPower(uint16_t *vbus, uint16_t *vshut, uint16_t *current, uint16_t *power);

#endif
