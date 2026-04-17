#ifndef _KEY_H
#define _KEY_H

#include <stdio.h>
#include "at32f421.h"
/* input */
#define CH1_PORT                GPIOA
#define CH1_PIN                 GPIO_PINS_3
#define READ_CH1                gpio_input_data_bit_read(CH1_PORT, CH1_PIN)

#define CH2_PORT                GPIOA
#define CH2_PIN                 GPIO_PINS_4
#define READ_CH2                gpio_input_data_bit_read(CH2_PORT, CH2_PIN)

#define CH3_PORT                GPIOA
#define CH3_PIN                 GPIO_PINS_5
#define READ_CH3                gpio_input_data_bit_read(CH3_PORT, CH3_PIN)

#define LONG_PRESS_TIME          5 
#define KEY_CYCLE_TIME           10
#define KEY_NUMBER               3
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    K_RELEASE,
    K_PRESS,
} KEY_VALUE;

typedef enum {
    KS_RELEASE,
	//KS_LONG_RELEASE,
	KS_CHECK,
    KS_PRESS,
	KS_LONG_PRESS,
} KEY_STATUS;

typedef enum {
    KE_PRESS,
    KE_RELEASE,
    KE_LONG_PRESS,
    KE_LONG_RELEASE,
    KE_NONE,
} KEY_EVENT;

typedef struct {
    KEY_STATUS status;
    int count;
	int key_cycle_time;
    KEY_VALUE (*get)(void);
} KEY;

KEY_EVENT key_event_check(KEY *key, int interval);

#ifdef __cplusplus
}
#endif
#endif


