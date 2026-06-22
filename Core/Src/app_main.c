/*
 * app_main.c
 * Task 1: KEY-controlled RGB state and optional buzzer state.
 */
#include "app_main.h"
#include "app_config.h"
#include "bsp_buzzer.h"
#include "bsp_key.h"
#include "bsp_rgb.h"
#include "retarget.h"
#include <stdio.h>

typedef enum
{
    TASK1_STATE_GREEN_ON = 0,
    TASK1_STATE_RED_ON,
    TASK1_STATE_GREEN_BREATH
} Task1State_t;

static Task1State_t g_task1_state = TASK1_STATE_GREEN_ON;
static uint16_t g_breath_duty = 0;
static int16_t g_breath_step = 20;
static uint32_t g_breath_tick = 0;
static uint8_t g_key2_override = 0;
static uint8_t g_rgb_off_after_key2 = 0;

/* Print current task state. */
static void APP_PrintState(void)
{
    if (g_task1_state == TASK1_STATE_GREEN_ON)
    {
        printf("[TASK1] State 0: RGB=GREEN_ON, BUZZER=ON\r\n");
    }
    else if (g_task1_state == TASK1_STATE_RED_ON)
    {
        printf("[TASK1] State 1: RGB=RED_ON, BUZZER=OFF\r\n");
    }
    else
    {
        printf("[TASK1] State 2: RGB=GREEN_BREATH, BUZZER=INTERVAL\r\n");
    }
}

/* Apply RGB and buzzer state. */
static void APP_ApplyState(void)
{
    g_rgb_off_after_key2 = 0;

    if (g_task1_state == TASK1_STATE_GREEN_ON)
    {
        RGB_SetGreen();
        Buzzer_SetState(BUZZER_ON);
    }
    else if (g_task1_state == TASK1_STATE_RED_ON)
    {
        RGB_SetRed();
        Buzzer_SetState(BUZZER_OFF);
    }
    else
    {
        g_breath_duty = 0;
        g_breath_step = 20;
        RGB_SetGreenBreath(g_breath_duty);
        Buzzer_SetState(BUZZER_BEEP_INTERVAL);
    }

    APP_PrintState();
}

/* Switch state after KEY1 short press. */
static void APP_NextState(void)
{
    if (g_task1_state == TASK1_STATE_GREEN_BREATH)
    {
        g_task1_state = TASK1_STATE_GREEN_ON;
    }
    else
    {
        g_task1_state = (Task1State_t)(g_task1_state + 1);
    }

    APP_ApplyState();
}

/* Non-blocking green breathing PWM update. */
static void APP_BreathTask(void)
{
    int32_t next_duty;

    if ((g_key2_override != 0) ||
        (g_rgb_off_after_key2 != 0) ||
        (g_task1_state != TASK1_STATE_GREEN_BREATH))
    {
        return;
    }

    if ((HAL_GetTick() - g_breath_tick) < BREATH_UPDATE_MS)
    {
        return;
    }

    g_breath_tick = HAL_GetTick();
    next_duty = (int32_t)g_breath_duty + g_breath_step;

    if (next_duty >= (int32_t)RGB_PWM_MAX)
    {
        next_duty = RGB_PWM_MAX;
        g_breath_step = -20;
    }
    else if (next_duty <= 0)
    {
        next_duty = 0;
        g_breath_step = 20;
    }

    g_breath_duty = (uint16_t)next_duty;
    RGB_SetGreenBreath(g_breath_duty);
}

/* Initialize Task 1 application. */
void APP_Init(void)
{
    RGB_Init();
    Buzzer_Init();
    BSP_Key_Init();

    printf("\r\n[SYS] STM32 Task1 RGB Key System Start\r\n");
    printf("[KEY] KEY1=PB12, KEY2=PB13\r\n");
    printf("[RGB] R=PB0, G=PB1 TIM3_CH4, B=PB10\r\n");
#if (USE_BUZZER != 0)
    printf("[BUZZER] enabled, PB11\r\n");
#else
    printf("[BUZZER] disabled\r\n");
#endif

    g_task1_state = TASK1_STATE_GREEN_ON;
    g_key2_override = 0;
    g_rgb_off_after_key2 = 0;
    APP_ApplyState();
}

/* Main application loop. */
void APP_Loop(void)
{
    uint8_t key_event;

    key_event = BSP_Key_Scan();

    if ((key_event & KEY2_EVENT_LONG_START) != 0)
    {
        g_key2_override = 1;
        RGB_SetGreen();
        printf("[TASK1] KEY2 long press: GREEN ON\r\n");
    }

    if ((key_event & KEY2_EVENT_LONG_RELEASE) != 0)
    {
        g_key2_override = 0;
        g_rgb_off_after_key2 = 1;
        RGB_AllOff();
        printf("[TASK1] KEY2 released: RGB OFF\r\n");
    }

    if ((g_key2_override == 0) && ((key_event & KEY1_EVENT_SHORT) != 0))
    {
        APP_NextState();
    }

    if (g_key2_override != 0)
    {
        RGB_SetGreen();
    }
    else
    {
        APP_BreathTask();
    }

    Buzzer_Task();
}
