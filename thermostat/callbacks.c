/*
 * callbacks.c
 *
 *  Created on: Oct 20, 2024
 *      Author: Bobby Rust
 */

#include <ti/drivers/Timer.h>

#include "callbacks.h"

volatile unsigned char TimerFlag   = 0;
volatile unsigned char Button0Flag = 0;
volatile unsigned char Button1Flag = 0;

void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    TimerFlag = 1;
}

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn0(uint_least8_t index)
{
    Button0Flag = 1;
}

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
void gpioButtonFxn1(uint_least8_t index)
{
    Button1Flag = 1;
}
