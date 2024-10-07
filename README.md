# TI CC3220S Projects

## GPIO Interrupt

Uses interrupts and timers to blink Morse code messages. Toggles Morse code message state with SW2. User LED D10 corresponds to DOT and blinks 500ms, D8 corresponds to DASH and blinks 1500ms. Period is set to 500ms, so the DASH state is active for 3 ticks. Character pauses are handled in the CPAUSE state which lasts 3 ticks. Word pauses are handled in the WPAUSE state and last 7 ticks. A timer interrupt is set up to trigger continuously over the specified period by setting the timerMode param to Timer_CONTINUOUS_CALLBACK. When SW2 is pressed, the Morse code message toggles between SOS and OK. If it is pressed in the middle of a message, the message finishes before switching. An interrupt triggers the gpioButtonFxn0, which corresponds to CONFIG_GPIO_BUTTON_0, which is set to SW2 in gpiointerrupt.syscfg.

![Alt text](./docs/images/morse-code-state-machine.png)

## UART2 Echo

Uses the CC3220S UART device to control User LEDs by sending messages ("On" or "Off") from serial terminal.

![Alt text](./docs/images/on-off-state-machine.png)

![Alt text](./docs/images/CC3220S-hardware-description.png)
CC3220 figure retrieved from _TI User's Guide CC3220 SimpleLink™ Wi-Fi® LaunchPad™ Development
Kit Hardware_. https://www.ti.com/lit/ug/swru463c/swru463c.pdf?ts=1728229889379
