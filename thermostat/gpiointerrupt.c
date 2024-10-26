/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <stddef.h>

#include "init.h"
#include "utils.h"

typedef enum States { STATE_IDLE, STATE_CHECKBTNFLAGS, STATE_READTEMP, STATE_SENDUARTDATA } States;

#define OFF 0
#define ON  1

void setHeat(int16_t temperature, int16_t setpoint, uint8_t* heat)
{
    if (temperature < setpoint)
    {
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
        *heat = ON;
    }
    else
    {
        *heat = OFF;
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    }
}

void checkButtonFlags(int16_t temperature, int16_t* setpoint, uint8_t* heat)
{
    if (Button0Flag)
    {
        (*setpoint)--;
    }
    if (Button1Flag)
    {
        (*setpoint)++;
    }

    setHeat(temperature, *setpoint, heat);
    Button0Flag = 0;
    Button1Flag = 0;
}

void sendUARTData(int16_t temperature, int16_t setpoint, uint8_t heat, uint32_t seconds)
{
    DISPLAY( snprintf(output, 64, "<%02d,%02d,%d,%04d>\r\n", temperature, setpoint, heat, seconds))
}

void TickFct(uint16_t* timer, int16_t* temperature, int16_t* setpoint, uint8_t* heat, uint32_t* seconds)
{
    States state = STATE_IDLE;

    // Transitions
    switch(*timer)
    {
    case 5:
        state = STATE_READTEMP;
        break;
    case 10:
        state = STATE_SENDUARTDATA;
        break;
    default:
        if (*timer % 2 == 0)
        {
            state = STATE_CHECKBTNFLAGS;
        }
        else
        {
            state = STATE_IDLE;
        }
        break;
    }

    // Actions
    switch(state)
    {
    case STATE_CHECKBTNFLAGS:
        checkButtonFlags(*temperature, setpoint, heat);
        break;
    case STATE_READTEMP:
        *temperature = readTemp();
        break;
    case STATE_SENDUARTDATA:
        checkButtonFlags(*temperature, setpoint, heat);
        *temperature = readTemp();
        (*seconds)++;
        sendUARTData(*temperature, *setpoint, *heat, *seconds);
        *timer = 0;
        break;
    case STATE_IDLE:
        // do nothing
        break;
    }

    // Lower flags
    TimerFlag = 0;
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    int16_t     temperature = 0;
    int16_t     setpoint = 21;
    uint16_t    timer = 0;
    uint8_t     heat = 0;
    uint32_t    seconds = 0;

    // Init
    initGPIO();
    initUART(); // The UART must be initialized before calling initI2C()

    DISPLAY( snprintf(output, 64, "UART + GPIO + Timer +I2C + Interrupts by Eric Gregori\n\r"))
    DISPLAY( snprintf(output, 64, "GPIO + Interrupts configured\n\r"))

    initI2C();
    initTimer();

    // Loop Forever
    // The student should add flags (similiar to the timer flag) to the button handlers.
    // Timer interrupt set to 100ms
    DISPLAY( snprintf(output, 64, "Starting Task Scheduler\n\r"))
    while (1)
    {
        while (!TimerFlag) {}; // Wait for timer period

        TickFct(&timer, &temperature, &setpoint, &heat, &seconds);

        ++timer;
    }

    return (NULL);
}
