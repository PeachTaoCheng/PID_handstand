#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
#include "gpio.h"
#include "tim.h"

#define AIN1_GPIO_Port    GPIOB
#define AIN1_Pin          GPIO_PIN_12

#define AIN2_GPIO_Port    GPIOB
#define AIN2_Pin          GPIO_PIN_13

#define AIN1_High()     HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
#define AIN1_Low()      HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);

#define AIN2_High()     HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
#define AIN2_Low()      HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);

void Motor_SetPWM(int16_t Motor_PWM);

#endif