#include "Encoder.h"

int16_t Encoder_GetValue(void)
{
    int16_t Encoder_Value =  __HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    return Encoder_Value;;

}