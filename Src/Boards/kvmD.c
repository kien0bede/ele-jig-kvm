#include "adapter_board.h"
#include "controller_board.h"

static PinInfo GND_DETECT_Pin = {
	.pin = GPIO_PIN_0,
	.port = GPIOB
};
/* Mod GND - Nguyen Thanh Kien*/
static PinInfo BUTTON_POR_Pin = {
	.pin = GPIO_PIN_15,
	.port = GPIOB,
};

static PinInfo BOOT0_SEL_Pin = {
	.pin = GPIO_PIN_10,
	.port = GPIOB,
};

static PinInfo BOOT1_SEL_Pin = {
	.pin = GPIO_PIN_11,
	.port = GPIOB,
};

static IRQn_Type g_elinkKVM_revD_IrqList[1] = { EXTI4_15_IRQn };

/* Refer to power_mgr.h to know how to provide test point specs */
static TestPointSpec g_testPointSpecs[] = {
	TEST_POINT_SPEC(5, "VDD_DRAM", ADC_INPUT_5, ADC_CHANNEL_5, false, true, 1.2f, 0.6f, 1.35f, 0.025f, 0.002f),
	TEST_POINT_SPEC(6, "VDD_ARM", ADC_INPUT_6, ADC_CHANNEL_6, false, true, 1.2f, 0.6f, 1.4f, 0.015f, 0.002f),
	TEST_POINT_SPEC(7, "DCDC_1V8", ADC_INPUT_7, ADC_CHANNEL_7, false, true, 1.2f, 0.6f, 1.8f, 0.025f, 0.006f),
	TEST_POINT_SPEC(9, "DCDC_3V3", ADC_INPUT_9, ADC_CHANNEL_9, false, true, 3.3f, 0.6f, 3.3f, 0.015f, 0.01f)
};
/* Mod TestPointSpec - Nguyen Thanh Kien*/
static void elinkKVM_revD_Init()
{
	/* Reset button initialization */
	GPIO_InitTypeDef buttonPor = {0};

	buttonPor.Pin = BUTTON_POR_Pin.pin;
	buttonPor.Mode = GPIO_MODE_IT_RISING_FALLING;
	buttonPor.Pull = GPIO_PULLUP;

	HAL_GPIO_Init(BUTTON_POR_Pin.port, &buttonPor);

	/* Boot select initialization */
	GPIO_InitTypeDef bootSel = {0};

	bootSel.Pin = BOOT0_SEL_Pin.pin | BOOT1_SEL_Pin.pin;
	bootSel.Mode = GPIO_MODE_OUTPUT_PP;
	bootSel.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(BOOT0_SEL_Pin.port, &bootSel);

	// TODO: disable PMIC_ON_REQ due to unknown hardware issues
	g_testPointSpecs[9].failureAccepted = true;
}

static void elinkKVM_revD_IRQ_Handler(IRQn_Type irq)
{
	if (irq != EXTI4_15_IRQn)
	{
		return;
	}

	if (__HAL_GPIO_EXTI_GET_IT(BUTTON_POR_Pin.pin) != 0x0)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(BUTTON_POR_Pin.pin);

		if (HAL_GPIO_ReadPin(BUTTON_POR_Pin.port, BUTTON_POR_Pin.pin) == GPIO_PIN_RESET)
		{
			CB_IRQ_OnTargetReset(true);
		}
		else
		{
			CB_IRQ_OnTargetReset(false);
		}
	}
}

static void elinkKVM_revD_EnterNewMode(OperationMode mode)
{
	switch (mode)
	{
	case OPMODE_AUTO:
		break;
	case OPMODE_CONTROL:
		break;
	default:
		break;
	}
}

static bool elinkKVM_revD_Handle_HID_OutReport(HID_OutReport *outRp)
{
	switch (outRp->type)
	{
	default:
		return false;
	}

	return true; // command handled
}

static bool elinkKVM_revD_SwitchBootMode(BootMode mode)
{
	switch (mode)
	{
	case BOOTMODE_SERIAL:
		HAL_GPIO_WritePin(BOOT0_SEL_Pin.port, BOOT0_SEL_Pin.pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(BOOT1_SEL_Pin.port, BOOT1_SEL_Pin.pin, GPIO_PIN_RESET);
		break;
	case BOOTMODE_DEVELOPMENT:
		HAL_GPIO_WritePin(BOOT0_SEL_Pin.port, BOOT0_SEL_Pin.pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BOOT1_SEL_Pin.port, BOOT1_SEL_Pin.pin, GPIO_PIN_SET);
		break;
	case BOOTMODE_FUSE:
		HAL_GPIO_WritePin(BOOT0_SEL_Pin.port, BOOT0_SEL_Pin.pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BOOT1_SEL_Pin.port, BOOT1_SEL_Pin.pin, GPIO_PIN_RESET);
		break;
	default:
		return false;
	}

	CB_OnBootModeChanged(mode);
	return true;
}

static AdapterBoard __board = {
	.boardName = "ElinkKVM revD",
	.irqList = g_elinkKVM_revD_IrqList,
	.irqListSz = sizeof(g_elinkKVM_revD_IrqList) / sizeof(g_elinkKVM_revD_IrqList[0]),
	.testPointSpecs = g_testPointSpecs,
	.testPointSpecsSz = sizeof(g_testPointSpecs) / sizeof(g_testPointSpecs[0]),
	.gndDetect = &GND_DETECT_Pin,
	.Init = elinkKVM_revD_Init,
	.EnterNewMode = elinkKVM_revD_EnterNewMode,
	.Handle_HID_OutReport = elinkKVM_revD_Handle_HID_OutReport,
	.SwitchBootMode = elinkKVM_revD_SwitchBootMode,
	.IRQ_Handler = elinkKVM_revD_IRQ_Handler
};
/* Mod AdapterBoard - Nguyen Thanh Kien*/
AdapterBoard *GetAdapterBoard(void)
{
	return &__board;
}
