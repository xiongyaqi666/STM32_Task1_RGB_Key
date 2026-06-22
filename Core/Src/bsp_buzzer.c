/*
 * bsp_buzzer.c
 * Optional buzzer driver. When USE_BUZZER is 0, functions keep state only.
 */
#include "bsp_buzzer.h"
#include "app_config.h"

static BuzzerState_t g_buzzer_state = BUZZER_OFF;
static uint32_t g_buzzer_tick = 0;
static uint8_t g_buzzer_level = 0;

#if (USE_BUZZER != 0)
/* Configure PB11 as push-pull output when buzzer hardware is enabled. */
static void Buzzer_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = BUZZER_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);
}

/* Write buzzer output level. */
static void Buzzer_Write(uint8_t on)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
#else
static void Buzzer_Write(uint8_t on)
{
    (void)on;
}
#endif

/* Initialize optional buzzer state. */
void Buzzer_Init(void)
{
#if (USE_BUZZER != 0)
    Buzzer_GPIO_Init();
#endif
    g_buzzer_state = BUZZER_OFF;
    g_buzzer_tick = HAL_GetTick();
    g_buzzer_level = 0;
    Buzzer_Write(0);
}

/* Set buzzer state. */
void Buzzer_SetState(BuzzerState_t state)
{
    g_buzzer_state = state;
    g_buzzer_tick = HAL_GetTick();

    if (state == BUZZER_ON)
    {
        g_buzzer_level = 1;
        Buzzer_Write(1);
    }
    else
    {
        g_buzzer_level = 0;
        Buzzer_Write(0);
    }
}

/* Non-blocking interval beep task. */
void Buzzer_Task(void)
{
    if (g_buzzer_state != BUZZER_BEEP_INTERVAL)
    {
        return;
    }

    if ((HAL_GetTick() - g_buzzer_tick) >= BUZZER_INTERVAL_MS)
    {
        g_buzzer_tick = HAL_GetTick();
        g_buzzer_level = (uint8_t)(g_buzzer_level == 0);
        Buzzer_Write(g_buzzer_level);
    }
}

/* Return current buzzer state. */
BuzzerState_t Buzzer_GetState(void)
{
    return g_buzzer_state;
}
