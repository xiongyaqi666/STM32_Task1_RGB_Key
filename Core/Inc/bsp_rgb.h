/*
 * bsp_rgb.h
 * RGB LED control, green channel uses TIM3_CH4 PWM.
 */
#ifndef __BSP_RGB_H__
#define __BSP_RGB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void RGB_Init(void);
void RGB_AllOff(void);
void RGB_SetRed(void);
void RGB_SetGreen(void);
void RGB_SetBlue(void);
void RGB_SetGreenBreath(uint16_t duty);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_RGB_H__ */
