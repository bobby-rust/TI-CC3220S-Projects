/*
 * init.h
 *
 *  Created on: Oct 20, 2024
 *      Author: Bobby Rust
 */

#ifndef INIT_H_
#define INIT_H_

#include <stdint.h>
#include <stdio.h>

#include "ti_drivers_config.h"

#include "callbacks.h"

#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART2.h>

#define DISPLAY(x) UART2_write(uart, &output, x, NULL);

// I2C Global Variables
static const struct {
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[3] = {
    { 0x48, 0x0000, "11X" },
    { 0x49, 0x0000, "116" },
    { 0x41, 0x0001, "006" }
};

uint8_t             txBuffer[1];
uint8_t             rxBuffer[2];
I2C_Transaction     i2cTransaction;

// Driver Handles - Global variables
I2C_Handle      i2c;
UART2_Handle    uart;
Timer_Handle    timer0;

// UART Global Variables
char                output[64];
int                 bytesToSend;


void initUART(void);
// Make sure you call initUART() before calling this function.
void initI2C(void);
void initTimer(void);
void initGPIO(void);

#endif /* INIT_H_ */
