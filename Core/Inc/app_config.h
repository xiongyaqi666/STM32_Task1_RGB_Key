/*
 * app_config.h
 * Task 1 central configuration.
 */
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Set to 1 only when a buzzer is connected to PB11. */
#define USE_BUZZER                     0

/* Key timing. */
#define KEY_DEBOUNCE_MS                20U
#define KEY_LONG_PRESS_MS              800U

/* RGB common-cathode module, high level lights the LED. */
#define RGB_ACTIVE_LEVEL               GPIO_PIN_SET
#define RGB_INACTIVE_LEVEL             GPIO_PIN_RESET

/* Pin mapping. */
#define RGB_R_GPIO_Port                GPIOB
#define RGB_R_Pin                      GPIO_PIN_0
#define RGB_G_GPIO_Port                GPIOB
#define RGB_G_Pin                      GPIO_PIN_1
#define RGB_B_GPIO_Port                GPIOB
#define RGB_B_Pin                      GPIO_PIN_10
#define BUZZER_GPIO_Port               GPIOB
#define BUZZER_Pin                     GPIO_PIN_11
#define KEY1_GPIO_Port                 GPIOB
#define KEY1_Pin                       GPIO_PIN_12
#define KEY2_GPIO_Port                 GPIOB
#define KEY2_Pin                       GPIO_PIN_13

/* Key is pull-up input, low level means pressed. */
#define KEY_PRESSED_LEVEL              GPIO_PIN_RESET

/* TIM3_CH4 PWM range. */
#define RGB_PWM_MAX                    999U
#define BREATH_UPDATE_MS               10U
#define BUZZER_INTERVAL_MS             500U

#ifdef __cplusplus
}
#endif

#endif /* __APP_CONFIG_H__ */
