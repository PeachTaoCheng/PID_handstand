#include "motor.h"
#include <stdint.h>
#include <stdlib.h>

void motor_foreward(void)
{
    AIN1_Low();
    AIN2_High();
}

void motor_backword(void)
{
    AIN1_High();
    AIN2_Low();
}

void motor_stop(void)
{
    AIN1_Low();
    AIN2_Low();
}

void Motor_SetPWM(int16_t Motor_PWM)
{
    if(Motor_PWM > 0)
    {
        // motor_backword();
        motor_foreward();
    }
    else if(Motor_PWM < 0)
    {
        // motor_foreward();
        motor_backword();
    }
    else
    {
        motor_stop();
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, abs(Motor_PWM));

}

