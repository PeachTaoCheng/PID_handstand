#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"
#include "gpio.h"

#define KEY_GPIO_Port GPIOB
#define KEY_Pin GPIO_PIN_10

uint8_t key_state(void);


#endif