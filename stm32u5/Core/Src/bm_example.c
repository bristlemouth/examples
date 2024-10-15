/*
 * bm_example.c
 *
 *  Created on: Sep 17, 2024
 *      Author: zac
 */

#include "adin2111.h"
#include "stm32u5xx_hal.h"

void bm_example_init(void) {
  // Power on the adin via the load switch
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_SET);

  adin2111_DeviceStruct_t adin;
  adin2111_DriverConfig_t adin_cfg;
  adi_eth_Result_e result = adin2111_Init(&adin, &adin_cfg);
}

void bm_example_update(void) {}
