/*
 * bm_example.c
 *
 *  Created on: Sep 17, 2024
 *      Author: zac
 */

#include "adin1110.h"
#include "stm32_dma.h"
#include "stm32_gpio.h"
#include "stm32_spi.h"
//#include "no_os_pwm.h"

extern DMA_HandleTypeDef handle_GPDMA1_Channel12;
extern DMA_HandleTypeDef handle_GPDMA1_Channel13;

void bm_example_init(void) {
  const struct no_os_gpio_init_param adin1110_reset_gpio_ip = {
      .port = 0,
      .number = 0,
      .pull = NO_OS_PULL_UP,
      .platform_ops = &stm32_gpio_ops,
      .extra = NULL,
  };
  struct no_os_dma_init_param dma_init = {
      .id = 0,
      .num_ch = 2,
      .platform_ops = &stm32_dma_ops,
  };
  struct stm32_dma_channel txdma_channel = {
      .hdma = &handle_GPDMA1_Channel13,
      .ch_num = GPDMA1_Channel13,
      .mem_increment = true,
      .per_increment = false,
      .mem_data_alignment = DATA_ALIGN_BYTE,
      .per_data_alignment = DATA_ALIGN_BYTE,
      .dma_mode = DMA_NORMAL_MODE
  };
  struct stm32_dma_channel rxdma_channel = {
      .hdma = &handle_GPDMA1_Channel12,
      .ch_num = GPDMA1_Channel12,
      .mem_increment = true,
      .per_increment = false,
      .mem_data_alignment = DATA_ALIGN_BYTE,
      .per_data_alignment = DATA_ALIGN_BYTE,
      .dma_mode = DMA_NORMAL_MODE,
  };
//  struct stm32_pwm_init_param cs_pwm_extra_init_params = {
//      .prescaler = 1,
//      .timer_autoreload = true,
//      .mode = TIM_OC_PWM1,
//      .timer_chn = 1,
//      .complementary_channel = false,
//      .get_timer_clock = HAL_RCC_GetPCLK1Freq,
//      .clock_divider = 2,
//      .onepulse_enable = true,
//      .trigger_enable = true,
//      .trigger_source = PWM_TS_ITR0,
//      .trigger_output = PWM_TRGO_ENABLE,
//  };
//  struct no_os_pwm_init_param cs_pwm_init = {
//      .id = 2,
//      /* time to xfer bytes */
//      .period_ns = CS_PWM_PERIOD_NS,
//      .duty_cycle_ns = CS_PWM_DUTY_NS,
//      .polarity = NO_OS_PWM_POLARITY_HIGH,
//      .platform_ops = &stm32_pwm_ops,
//      .extra = &cs_pwm_extra_init_params,
//  };
//  struct stm32_pwm_init_param tx_pwm_extra_init_params = {
//      .prescaler = 1,
//      .timer_autoreload = true,
//      .mode = TIM_OC_PWM1,
//      .timer_chn = 1,
//      .complementary_channel = false,
//      .get_timer_clock = HAL_RCC_GetPCLK2Freq,
//      .clock_divider = 2,
//      .onepulse_enable = true,
//      .trigger_enable = true,
//      .trigger_source = PWM_TS_ITR0,
//      .dma_enable = true,
//      .repetitions = TX_PWM_REPS,
//  };
//  struct no_os_pwm_init_param tx_pwm_init = {
//      .id = 8,
//      /* time to xfer 8 bits */
//      .period_ns = TX_PWM_PERIOD_NS,
//      .duty_cycle_ns = TX_PWM_DUTY_NS,
//      .polarity = NO_OS_PWM_POLARITY_HIGH,
//      .platform_ops = PWM_OPS,
//      .extra = &tx_pwm_extra_init_params};
  struct stm32_spi_init_param spi_extra_ip = {
      .chip_select_port = 0, // CS is PA15, GPIO Port A == 0
      .get_input_clock = HAL_RCC_GetPCLK2Freq,
      .dma_init = &dma_init,
      .txdma_ch = &txdma_channel,
      .rxdma_ch = &rxdma_channel,
      .irq_num = GPDMA1_Channel12_IRQn,
      /* TODO: If adin works with these null, delete the commented structs above */
      .pwm_init = NULL,
      .tx_pwm_init = NULL,
      .alternate = GPIO_AF1_TIM1,
  };
  const struct no_os_spi_init_param adin1110_spi_ip = {
      .device_id = 3,
      .max_speed_hz = 20000000,
      .bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
      .mode = NO_OS_SPI_MODE_0,
      .platform_ops = &stm32_spi_ops,
      .chip_select = 15, // ?? PA15
      .extra = &spi_extra_ip,
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
