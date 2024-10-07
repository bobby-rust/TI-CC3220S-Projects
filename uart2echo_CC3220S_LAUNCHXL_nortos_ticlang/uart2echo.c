/*
 * Copyright (c) 2020, Texas Instruments Incorporated
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
 *  ======== uart2echo.c ========
 */
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>

/* Driver configuration */
#include "ti_drivers_config.h"

// Transition checks
#define CHECK_O *input == 'O' || *input == 'o'
#define CHECK_N *input == 'N' || *input == 'n'
#define CHECK_F *input == 'F' || *input == 'f'

// States
#define STATE_OFF  0
#define STATE_ON   1
#define STATE_READ 2
#define STATE_O    3
#define STATE_OF   4

void ledOn()
{
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
}

void ledOff()
{
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
}

/**
 * Reads a single char from UART channel into a buffer
 * @param {UART2_Handle *uart} handle to the uart channel to read from
 * @param {char *buffer} the buffer to read into
 * @return void
 */
void readChar(UART2_Handle *uart, char* buffer)
{
    size_t bytesRead = 0;
    uint32_t status = UART2_read(*uart, buffer, 1, &bytesRead);
    if (status != UART2_STATUS_SUCCESS)
    {
        // Some issue, end code execution
        while (1);
    }
}

void writeChar(UART2_Handle *uart, char *buffer)
{
    size_t bytesWritten = 0;
    uint32_t status = UART2_write(*uart, buffer, 1, &bytesWritten);

    if (status != UART2_STATUS_SUCCESS)
    {
        // Some issue, end code execution
        while (1);
    }
}

/* enum takes 4 bytes of memory, so it is not used, but I'm leaving this here for clarity */
// enum LEDStates { OFF = 0, ON = 1, IDLE = 2, O = 3, OF = 4 };

void ledTick(UART2_Handle *uart, char *input, unsigned char *state)
{
    /**
     * State transitions
     */
    switch (*state)
    {
    case STATE_OFF:
        *state = STATE_READ; // goto idle
        break;

    case STATE_ON:
        *state = STATE_READ; // goto idle
        break;

    case STATE_READ:
        if (CHECK_O) // if we are at IDLE, do nothing unless input is 'O'
        {
            *state = STATE_O;
        }
        break;

    case STATE_O:
        if (CHECK_N)
        {
            *state = STATE_ON;
        }
        else if (CHECK_F)
        {
            *state = STATE_OF;
        }
        else if (CHECK_O)
        {
            *state = STATE_O;
        }
        else
        {
            *state = STATE_READ;
        }
        break;

    case STATE_OF:
        *state = (CHECK_F) ? STATE_OFF : STATE_READ; // if we are at 'OF' and input is 'F' set state to OFF, else goto idle
        break;
    }

    /**
     * State actions
     */
    switch(*state)
    {
    case STATE_OFF:
        ledOff();
        break;
    case STATE_ON:
        ledOn();
        break;
    default:
        // These state actions happen in every state
        readChar(uart, input);
        writeChar(uart, input);
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    char input;
    unsigned char state = 2; // IDLE
    UART2_Handle uart;
    UART2_Params uartParams;
    uint32_t status = UART2_STATUS_SUCCESS;

    /* Call driver init functions */
    GPIO_init();

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Create a UART where the default read and write mode is BLOCKING */
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 115200;

    uart = UART2_open(CONFIG_UART2_0, &uartParams);

    if (uart == NULL)
    {
        /* UART2_open() failed */
        while (1) {}
    }

    const char echoPrompt[] = "Echoing...\r\n";
    size_t bytesWritten = 0;
    UART2_write(uart, echoPrompt, sizeof(echoPrompt), &bytesWritten);

    /* Turn on user LED to indicate successful initialization */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);

    // Initialize input
    readChar(&uart, &input);
    writeChar(&uart, &input);

    /* Loop forever echoing */
    while (1)
    {
        ledTick(&uart, &input, &state);
    }
}
