/*
 * utils.h
 *
 *  Created on: Oct 20, 2024
 *      Author: Bobby Rust
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#include "init.h"

#define DISPLAY(x) UART2_write(uart, &output, x, NULL);

int16_t readTemp(void);

#endif /* UTILS_H_ */
