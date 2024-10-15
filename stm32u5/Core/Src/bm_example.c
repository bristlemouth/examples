/*
 * bm_example.c
 *
 *  Created on: Sep 17, 2024
 *      Author: zac
 */

#include "adin2111.h"
#include "stm32u5xx_hal.h"

static uint8_t ADIN_DEVICE_MEMORY[ADIN2111_DEVICE_SIZE];

void bm_example_init(void) {
  // Power on the adin via the load switch
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_1, GPIO_PIN_SET);
  // Deassert chip select
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  // Put the ADIN in reset
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_Delay(1);
  // Bring the ADIN out of reset
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  HAL_Delay(100);

  adin2111_DeviceStruct_t adin;
  adin2111_DriverConfig_t adin_cfg = {
      .pDevMem = &ADIN_DEVICE_MEMORY,
      .devMemSize = ADIN2111_DEVICE_SIZE,
      .fcsCheckEn = false,
      .tsTimerPin = ADIN2111_TS_TIMER_MUX_NA,
      .tsCaptPin = ADIN2111_TS_CAPT_MUX_NA,
  };
  adin2111_Init(&adin, &adin_cfg);
}

void bm_example_update(void) {}
