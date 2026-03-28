#include "key.h"

extern volatile uint32_t System_timer;

uint8_t key_state(void)
{
    uint8_t key_gpio_state  = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
    static uint32_t last_timer = 0;
    static uint8_t key_state = 0;
    switch (key_state) {
        case 0:
            if (key_gpio_state == GPIO_PIN_RESET)
            {
                last_timer = System_timer;
                key_state = 1;
            }
            break;
        case 1:
            if(System_timer - last_timer >= 10)
            {
                if(key_gpio_state == GPIO_PIN_RESET)
                {
                    key_state = 2;
                    
                }
                else
                {
                    key_state = 0;
                }
            }
            break;
        case 2:
            if(key_gpio_state == GPIO_PIN_SET)
            {
                last_timer = System_timer;
                key_state = 3;
            }
            break;
        case 3:
            if(System_timer - last_timer >= 10)
            {
                if(key_gpio_state == GPIO_PIN_SET)
                {
                    key_state = 0;
                    return 1;
                    
                }
                else
                {
                    key_state = 2;
                }
            }
            break;


        default:
            {
            key_state = 0;
            break;
            }
    }
    return 0;
}