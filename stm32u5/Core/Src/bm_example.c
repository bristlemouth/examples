/*
 * bm_example.c
 *
 *  Created on: Sep 17, 2024
 *      Author: zac
 */

#include "adin1110.h"
#include "stm32_gpio.h"
#include "stm32_spi.h"

void bm_example_init(void) {
  const struct no_os_gpio_init_param adin1110_reset_gpio_ip = {
      .port = 0,
      .number = 0,
      .pull = NO_OS_PULL_UP,
      .platform_ops = &stm32_gpio_ops,
      .extra = NULL,
  };

  const struct no_os_spi_init_param adin1110_spi_ip = {
      .device_id = 3,
      .max_speed_hz = 20000000,
      .bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
      .mode = NO_OS_SPI_MODE_0,
      .platform_ops = &stm32_spi_ops,
      .chip_select = 15, // ?? PA15
      .extra = NULL,
  };

  struct adin1110_desc *adin1110;
  struct adin1110_init_param adin1110_ip = {
      .chip_type = ADIN2111,
      .comm_param = adin1110_spi_ip,
      .reset_param = adin1110_reset_gpio_ip,
      .append_crc = false,
  };
  adin1110_init(&adin1110, &adin1110_ip);
}

void bm_example_update(void) {}
