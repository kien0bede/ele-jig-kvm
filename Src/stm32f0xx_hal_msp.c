#include "stm32f0xx_hal.h"

void HAL_MspInit(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1) {
		GPIO_InitTypeDef pinConfig = {0};

		pinConfig.Pin = GPIO_PIN_8 | GPIO_PIN_9;
		pinConfig.Mode = GPIO_MODE_AF_OD;
		pinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
		pinConfig.Pull = GPIO_PULLUP;
		pinConfig.Alternate = GPIO_AF1_I2C1;

		HAL_GPIO_Init(GPIOB, &pinConfig);

		__HAL_RCC_I2C1_CLK_ENABLE();
	}
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
	if (htim_pwm->Instance == TIM15)
	{
		__HAL_RCC_TIM15_CLK_ENABLE();
	}
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1)
	{
		__HAL_RCC_ADC1_CLK_ENABLE();
	}
}
