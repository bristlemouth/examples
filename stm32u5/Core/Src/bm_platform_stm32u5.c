/*
 * bm_platform_stm32u5.c
 *
 *  Created on: Sep 24, 2024
 *      Author: zac
 */

#include "no_os_gpio.h"
#include "no_os_spi.h"
#include <stdint.h>

/* Obtain optional GPIO descriptor. */
int32_t no_os_gpio_get_optional(struct no_os_gpio_desc **desc,
                                const struct no_os_gpio_init_param *param) {
  return 0;
}

/* Enable the output direction of the specified GPIO. */
int32_t no_os_gpio_direction_output(struct no_os_gpio_desc *desc,
                                    uint8_t value) {
  return 0;
}

/* Set the value of the specified GPIO. */
int32_t no_os_gpio_set_value(struct no_os_gpio_desc *desc, uint8_t value) {
  return 0;
}

/* Free the resources allocated by no_os_gpio_get(). */
int32_t no_os_gpio_remove(struct no_os_gpio_desc *desc) { return 0; }

/* Initialize the SPI communication peripheral. */
int32_t no_os_spi_init(struct no_os_spi_desc **desc,
                       const struct no_os_spi_init_param *param) {
  return 0;
}

/* Iterate over the spi_msg array and send all messages at once */
int32_t no_os_spi_transfer(struct no_os_spi_desc *desc,
                           struct no_os_spi_msg *msgs, uint32_t len) {
  return 0;
}

/* Free the resources allocated by no_os_spi_init(). */
int32_t no_os_spi_remove(struct no_os_spi_desc *desc) { return 0; }
