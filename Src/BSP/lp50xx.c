#include "lp50xx.h"
#include "i2c_compat.h"

#define DEVICE_ADDRESS	((uint8_t)0x28U)

void LP50XX_ConfigDevice(const LP50XX_ConfigTypeDef *conf)
{
	uint8_t config1;
	if (!I2C_MemRead(DEVICE_ADDRESS, REG_CONFIG1, &config1, sizeof(config1))) {
		return;
	}

	if (conf->maxCurrent == LP50XX_CONFIG_MAX_CURRENT_35_mA) {
		config1 |= CONFIG1_MAX_CURRENT;
	} else {
		config1 &= ~CONFIG1_MAX_CURRENT;
	}

	if (conf->PWM_DitheringEn) {
		config1 |= CONFIG1_PWM_DITHERING_EN;
	} else {
		config1 &= ~CONFIG1_PWM_DITHERING_EN;
	}

	if (conf->autoIncrEn) {
		config1 |= CONFIG1_AUTO_INCR_EN;
	} else {
		config1 &= ~CONFIG1_AUTO_INCR_EN;
	}

	if (conf->powerSaveEn) {
		config1 |= CONFIG1_POWER_SAVE_EN;
	} else {
		config1 &= ~CONFIG1_POWER_SAVE_EN;
	}

	if (conf->logScaleEn) {
		config1 |= CONFIG1_LOG_SCALE_EN;
	} else {
		config1 &= ~CONFIG1_LOG_SCALE_EN;
	}

	I2C_MemWrite(DEVICE_ADDRESS, REG_CONFIG1, &config1, sizeof(config1));
}

void LP50XX_SetChipEnabled(bool enabled)
{
	uint8_t config0;
	if (!I2C_MemRead(DEVICE_ADDRESS, REG_CONFIG0, &config0, sizeof(config0))) {
		return;
	}

	if (enabled) {
		config0 |= CONFIG0_CHIP_EN;
	} else {
		config0 &= ~CONFIG0_CHIP_EN;
	}

	I2C_MemWrite(DEVICE_ADDRESS, REG_CONFIG0, &config0, sizeof(config0));
}

void LP50XX_DisableAllLeds(bool yes)
{
	uint8_t config1;
	if (!I2C_MemRead(DEVICE_ADDRESS, REG_CONFIG1, &config1, sizeof(config1))) {
		return;
	}

	if (yes) {
		config1 |= CONFIG1_LED_GLOBAL_OFF;
	} else {
		config1 &= ~CONFIG1_LED_GLOBAL_OFF;
	}

	I2C_MemWrite(DEVICE_ADDRESS, REG_CONFIG1, &config1, sizeof(config1));
}

void LP50XX_SetLedBankEnabled(uint8_t ledBanks, bool enabled)
{
	uint8_t ledConfig0;
	if (!I2C_MemRead(DEVICE_ADDRESS, REG_LED_CONFIG0, &ledConfig0, sizeof(ledConfig0))) {
		return;
	}

	if (enabled) {
		ledConfig0 |= ledBanks;
	} else {
		ledConfig0 &= ~ledBanks;
	}

	I2C_MemWrite(DEVICE_ADDRESS, REG_LED_CONFIG0, &ledConfig0, sizeof(ledConfig0));
}

void LP50XX_SetBankBrightness(uint8_t brightness)
{
	I2C_MemWrite(DEVICE_ADDRESS, REG_BANK_BRIGHTNESS, &brightness, sizeof(brightness));
}

void LP50XX_SetBankColor(uint8_t bank, uint8_t color)
{
	if (bank >= LP50XX_BANK_A && bank <= LP50XX_BANK_C) {
		I2C_MemWrite(DEVICE_ADDRESS, bank, &color, sizeof(color));
	}
}

void LP50XX_SetLedBrightness(uint8_t led, uint8_t color)
{
#if defined(LP5012)
	if (led >= LP50XX_LED0_BRIGHTNESS && led <= LP50XX_LED3_BRIGHTNESS) {
#else
	if (led >= LP50XX_LED0_BRIGHTNESS && led <= LP50XX_LED2_BRIGHTNESS) {
#endif
		I2C_MemWrite(DEVICE_ADDRESS, led, &color, sizeof(color));
	}
}

void LP50XX_SetOutColor(uint8_t out, uint8_t color)
{
#if defined(LP5012)
	if (out >= LP50XX_OUT0 && out <= LP50XX_OUT11) {
#else
	if (out >= LP50XX_OUT0 && out <= LP50XX_OUT8) {
#endif
		I2C_MemWrite(DEVICE_ADDRESS, out, &color, sizeof(color));
	}
}

void LP50XX_Reset()
{
	uint8_t resetVal = 0xFFU;
	I2C_MemWrite(DEVICE_ADDRESS, REG_RESET, &resetVal, sizeof(resetVal));
	resetVal = 0x00U;
	I2C_MemWrite(DEVICE_ADDRESS, REG_RESET, &resetVal, sizeof(resetVal));
}
