#include "RP.h"

uint32_t RP_GetValue(uint8_t RP_Pin)
{
    uint32_t RP_Value;

    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    if(RP_Pin == 1)
    {
        sConfig.Channel = ADC_CHANNEL_8;
        while(HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK);
        HAL_ADC_Start(&hadc1);
        if(HAL_ADC_PollForConversion(&hadc1 , 10) == HAL_OK)
        {
        RP_Value = HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);
    }
    else if(RP_Pin == 2)
    {
        sConfig.Channel = ADC_CHANNEL_3;
        while(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK);
        HAL_ADC_Start(&hadc2);
        if(HAL_ADC_PollForConversion(&hadc2 , 10) == HAL_OK)
        {
        RP_Value = HAL_ADC_GetValue(&hadc2);
        }
        HAL_ADC_Stop(&hadc2);
    }
    else if(RP_Pin == 3)
    {
        sConfig.Channel = ADC_CHANNEL_4;
        while(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK);
        HAL_ADC_Start(&hadc2);
        if(HAL_ADC_PollForConversion(&hadc2 , 10) == HAL_OK)
        {
        RP_Value = HAL_ADC_GetValue(&hadc2);
        }
        HAL_ADC_Stop(&hadc2);
    }
    else if(RP_Pin == 4)
    {
        sConfig.Channel = ADC_CHANNEL_5;
        while(HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK);
        HAL_ADC_Start(&hadc2);
        if(HAL_ADC_PollForConversion(&hadc2 , 10) == HAL_OK)
        {
        RP_Value = HAL_ADC_GetValue(&hadc2);
        }
        HAL_ADC_Stop(&hadc2);
    }
    return RP_Value;
}