/*
 * callbacks.h
 *
 *  Created on: Oct 20, 2024
 *      Author: Bobby Rust
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <stdint.h>
#include <ti/drivers/Timer.h>

extern volatile unsigned char TimerFlag;
void timerCallback(Timer_Handle myHandle, int_fast16_t status);

/*
 *  ======== gpioButtonFxn0 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_0.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
extern volatile unsigned char Button0Flag;
void gpioButtonFxn0(uint_least8_t index);

/*
 *  ======== gpioButtonFxn1 ========
 *  Callback function for the GPIO interrupt on CONFIG_GPIO_BUTTON_1.
 *  This may not be used for all boards.
 *
 *  Note: GPIO interrupts are cleared prior to invoking callbacks.
 */
extern volatile unsigned char Button1Flag;
void gpioButtonFxn1(uint_least8_t index);

#endif /* CALLBACKS_H_ */
