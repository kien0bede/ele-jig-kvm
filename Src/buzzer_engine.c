#include "buzzer_engine.h"
#include "config.h"

enum BuzzerState {
	BUZZER_OFF,
	BUZZER_RESUMED,
	BUZZER_PAUSED
};

static PinInfo BUZZER_Pin = {
	.pin = CONFIG_BUZZER_GPIO_PIN,
	.port = CONFIG_BUZZER_GPIO_PORT
};

static uint32_t g_state = BUZZER_OFF;
static uint32_t g_nextDeadLine;
static BuzzerScript g_buzzerScript;

TIM_HandleTypeDef g_buzzer_timer;

static inline void StartBuzzer()
{
	HAL_TIM_PWM_Start(&g_buzzer_timer, CONFIG_BUZZER_CHANNEL);
	__HAL_TIM_SET_COMPARE(&g_buzzer_timer, CONFIG_BUZZER_CHANNEL, CONFIG_BUZZER_DUTY);
}

static inline void StopBuzzer()
{
	HAL_TIM_PWM_Stop(&g_buzzer_timer, CONFIG_BUZZER_CHANNEL);
}

void BZ_Init()
{
	/* Pin config */
	GPIO_InitTypeDef initStruct = {0};

	initStruct.Pin = BUZZER_Pin.pin;
	initStruct.Mode = GPIO_MODE_AF_PP;
	initStruct.Pull = GPIO_NOPULL;
	initStruct.Speed = GPIO_SPEED_FREQ_LOW;
	initStruct.Alternate = CONFIG_BUZZER_GPIO_AF_TIMER;

	HAL_GPIO_WritePin(BUZZER_Pin.port, BUZZER_Pin.pin, GPIO_PIN_RESET);
	HAL_GPIO_Init(BUZZER_Pin.port, &initStruct);

	/* Timer configs */

	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	g_buzzer_timer.Instance = CONFIG_BUZZER_TIMER;
	g_buzzer_timer.Init.Prescaler = 16;
	g_buzzer_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	g_buzzer_timer.Init.Period = 1023;
	g_buzzer_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	g_buzzer_timer.Init.RepetitionCounter = 0;
	g_buzzer_timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_PWM_Init(&g_buzzer_timer) != HAL_OK)
	{
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&g_buzzer_timer, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	if (HAL_TIM_PWM_ConfigChannel(&g_buzzer_timer, &sConfigOC, CONFIG_BUZZER_CHANNEL) != HAL_OK)
	{
		Error_Handler();
	}

	StopBuzzer();

	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

	if (HAL_TIMEx_ConfigBreakDeadTime(&g_buzzer_timer, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
}

void BZ_IRQ_SysTick_Handler()
{
	if (g_state == BUZZER_OFF)
	{
		return;
	}

	uint32_t now = HAL_GetTick();

	if (g_state == BUZZER_RESUMED)
	{
		if (now >= g_nextDeadLine)
		{
			StopBuzzer();

			g_nextDeadLine = now + MS_TO_TICK(g_buzzerScript.breakTime);
			g_state = BUZZER_PAUSED;

			if (g_buzzerScript.count > 1)
			{
				g_buzzerScript.count--;
			}
			else
			{
				g_state = BUZZER_OFF;
			}
		}
	}
	else if (g_state == BUZZER_PAUSED)
	{
		if (now >= g_nextDeadLine)
		{
			StartBuzzer();

			g_nextDeadLine = now + MS_TO_TICK(g_buzzerScript.interval);
			g_state = BUZZER_RESUMED;
		}
	}
}

void BZ_Play(BuzzerScript script)
{
	if (g_state == BUZZER_OFF) {
		StartBuzzer();
		g_buzzerScript = script;
		g_nextDeadLine = HAL_GetTick() + MS_TO_TICK(g_buzzerScript.interval);
		g_state = BUZZER_RESUMED;
	}
}

void BZ_Stop()
{
	if (g_state != BUZZER_OFF) {
		StopBuzzer();
		g_state = BUZZER_OFF;
	}
}
