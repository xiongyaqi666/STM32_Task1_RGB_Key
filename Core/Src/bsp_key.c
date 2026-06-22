/*
 * bsp_key.c
 * Software debounce, KEY1 short press, KEY2 long-hold detection.
 */
#include "bsp_key.h"
#include "app_config.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    GPIO_PinState stable_state;
    GPIO_PinState last_sample;
    uint32_t last_change_tick;
    uint32_t press_tick;
    uint8_t long_reported;
} KeyState_t;

static KeyState_t g_key1;
static KeyState_t g_key2;
static uint8_t g_key2_long_active = 0;

/* Initialize one key state from the current GPIO level. */
static void BSP_Key_InitOne(KeyState_t *key, GPIO_TypeDef *port, uint16_t pin)
{
    key->port = port;
    key->pin = pin;
    key->stable_state = HAL_GPIO_ReadPin(port, pin);
    key->last_sample = key->stable_state;
    key->last_change_tick = HAL_GetTick();
    key->press_tick = 0;
    key->long_reported = 0;
}

/* Initialize key debounce state machines. */
void BSP_Key_Init(void)
{
    BSP_Key_InitOne(&g_key1, KEY1_GPIO_Port, KEY1_Pin);
    BSP_Key_InitOne(&g_key2, KEY2_GPIO_Port, KEY2_Pin);
    g_key2_long_active = 0;
}

/* Scan KEY1: short press triggers once after release. */
static uint8_t BSP_Key_ScanKey1(void)
{
    GPIO_PinState sample;
    uint32_t now;
    uint8_t event;

    sample = HAL_GPIO_ReadPin(g_key1.port, g_key1.pin);
    now = HAL_GetTick();
    event = KEY_EVENT_NONE;

    if (sample != g_key1.last_sample)
    {
        g_key1.last_sample = sample;
        g_key1.last_change_tick = now;
    }

    if ((now - g_key1.last_change_tick) >= KEY_DEBOUNCE_MS)
    {
        if (sample != g_key1.stable_state)
        {
            g_key1.stable_state = sample;
            if (g_key1.stable_state == KEY_PRESSED_LEVEL)
            {
                g_key1.press_tick = now;
            }
            else
            {
                event = KEY1_EVENT_SHORT;
            }
        }
    }

    return event;
}

/* Scan KEY2: report long-start once and long-release after release. */
static uint8_t BSP_Key_ScanKey2(void)
{
    GPIO_PinState sample;
    uint32_t now;
    uint8_t event;

    sample = HAL_GPIO_ReadPin(g_key2.port, g_key2.pin);
    now = HAL_GetTick();
    event = KEY_EVENT_NONE;

    if (sample != g_key2.last_sample)
    {
        g_key2.last_sample = sample;
        g_key2.last_change_tick = now;
    }

    if ((now - g_key2.last_change_tick) >= KEY_DEBOUNCE_MS)
    {
        if (sample != g_key2.stable_state)
        {
            g_key2.stable_state = sample;
            if (g_key2.stable_state == KEY_PRESSED_LEVEL)
            {
                g_key2.press_tick = now;
                g_key2.long_reported = 0;
            }
            else
            {
                if (g_key2_long_active != 0)
                {
                    event = KEY2_EVENT_LONG_RELEASE;
                }
                g_key2_long_active = 0;
                g_key2.long_reported = 0;
            }
        }
    }

    if ((g_key2.stable_state == KEY_PRESSED_LEVEL) &&
        (g_key2.long_reported == 0) &&
        ((now - g_key2.press_tick) >= KEY_LONG_PRESS_MS))
    {
        g_key2.long_reported = 1;
        g_key2_long_active = 1;
        event = KEY2_EVENT_LONG_START;
    }

    return event;
}

/* Scan all keys and return event bits. */
uint8_t BSP_Key_Scan(void)
{
    uint8_t event;

    event = BSP_Key_ScanKey1();
    event |= BSP_Key_ScanKey2();
    return event;
}

/* Return whether KEY2 is currently in long-hold override state. */
uint8_t BSP_Key_IsKey2LongActive(void)
{
    return g_key2_long_active;
}
