/*
 * bsp_rgb.c
 * RGB control. Green LED brightness is controlled by TIM3_CH4 PWM on PB1.
 */
#include "bsp_rgb.h"
#include "app_config.h"
#include "tim.h"

/* Write one GPIO LED channel according to active-high logic. */
static void RGB_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t on)
{
    HAL_GPIO_WritePin(port, pin, on ? RGB_ACTIVE_LEVEL : RGB_INACTIVE_LEVEL);
}

/* Initialize RGB output state and start TIM3_CH4 PWM. */
void RGB_Init(void)
{
    (void)HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    RGB_AllOff();
}

/* Turn all RGB channels off. */
void RGB_AllOff(void)
{
    RGB_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, 0);
    RGB_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

/* Red LED constant on. */
void RGB_SetRed(void)
{
    RGB_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, 1);
    RGB_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

/* Green LED constant on. */
void RGB_SetGreen(void)
{
    RGB_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, 0);
    RGB_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, RGB_PWM_MAX);
}

/* Blue LED constant on. */
void RGB_SetBlue(void)
{
    RGB_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, 0);
    RGB_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, 1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

/* Set green PWM duty for breathing effect. */
void RGB_SetGreenBreath(uint16_t duty)
{
    if (duty > RGB_PWM_MAX)
    {
        duty = RGB_PWM_MAX;
    }

    RGB_WritePin(RGB_R_GPIO_Port, RGB_R_Pin, 0);
    RGB_WritePin(RGB_B_GPIO_Port, RGB_B_Pin, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, duty);
}
