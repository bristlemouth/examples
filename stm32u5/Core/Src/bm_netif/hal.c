/*
 *---------------------------------------------------------------------------
 *
 * Copyright (c) 2020, 2021 Analog Devices, Inc. All Rights Reserved.
 * This software is proprietary to Analog Devices, Inc.
 * and its licensors.By using this software you agree to the terms of the
 * associated Analog Devices Software License Agreement.
 *
 *---------------------------------------------------------------------------
 */

/** @addtogroup hal HAL API
 *  @{
 */

#include <string.h>
#include "hal.h"
#include "hal_port_specific.h"
#include "main.h"

static HAL_Callback_t INTERRUPT_CALLBACK = NULL;
static void *INTERRUPT_CB_PARAM = NULL;

#ifdef MDIO_GPIO
#include "../../mdio_gpio/mdio_gpio.h"
#endif

#ifdef ADIN1100
/*
 * @brief MDIO Read Clause 45
 *
 * @param [in]  phyAddr - Hardware PHY address
 * @param [in]  phyReg - Register address in clause 45 combined devType and regAddr
 * @param [out] phyData - pointer to the data buffer
 *
 * @return error if TA bit is not pulled down by the slave
 *
 * @details
 *
 * @sa
 */
uint32_t HAL_PhyRead(uint8_t hwAddr, uint32_t RegAddr, uint16_t *data)
{
#ifdef MDIO_GPIO
    return (uint32_t)mdioGPIORead_cl45(hwAddr, RegAddr, data);
#else
    return (uint32_t)adi_MdioRead_Cl45(hwAddr, RegAddr, data);
#endif
}

/*
 * @brief MDIO Write Clause 45
 *
 * @param [in] phyAddr - Hardware Phy address
 * @param [in] phyReg - Register address in clause 45 combined devAddr and regAddr
 * @param [out] phyData -  data
 * @return none
 *
 * @details
 *
 * @sa
 */
uint32_t HAL_PhyWrite(uint8_t hwAddr, uint32_t RegAddr, uint16_t data)
{
#ifdef MDIO_GPIO
  return mdioGPIOWrite_cl45(hwAddr, RegAddr, data);
#else
  return adi_MdioWrite_Cl45(hwAddr, RegAddr, data);
#endif
}
#endif

uint32_t HAL_DisableIrq(void)
{
    HAL_NVIC_DisableIRQ(ADIN_INT_EXTI_IRQn);

    return ADI_HAL_SUCCESS;
}

uint32_t HAL_EnableIrq(void)
{
    HAL_NVIC_EnableIRQ(ADIN_INT_EXTI_IRQn);

    return ADI_HAL_SUCCESS;
}

uint32_t HAL_SetPendingIrq(void)
{
    HAL_NVIC_SetPendingIRQ(ADIN_INT_EXTI_IRQn);

    return ADI_HAL_SUCCESS;
}

uint32_t HAL_GetPendingIrq(void)
{
    return HAL_NVIC_GetPendingIRQ(ADIN_INT_EXTI_IRQn);
}

uint32_t HAL_GetEnableIrq(void)
{
    return NVIC_GetEnableIRQ(ADIN_INT_EXTI_IRQn);
}


/*
 * @brief  Register Phy IRQ Callback function
 *
 * @param [in] intCallback
 * @param [in] hDevice - Pointer to Phy device handler
 * @param [out] none
 * @return none
 *
 * @details
 *
 * @sa
 */
uint32_t HAL_RegisterCallback(HAL_Callback_t const *intCallback, void * hDevice)
{
    INTERRUPT_CALLBACK = *intCallback;
    INTERRUPT_CB_PARAM = hDevice;
    return 0;
}

void adin_interrupt_handler()
{
    if (INTERRUPT_CALLBACK)
    {
        INTERRUPT_CALLBACK(INTERRUPT_CB_PARAM, 0, NULL);
    }
}

/*
 * @brief SPI write/read
 *
 * @param [in] pBufferTx - Pointer to transmit buffer
 * @param [in] nbBytes - Number bytes to send
 * @param [in] useDma - Enable/Disable DMA transfer for SPI
 * @param [out] pBufferRx - Pointer to receive buffer
 * @return none
 *
 * @details
 *
 * @sa
 */
uint32_t HAL_SpiReadWrite(uint8_t *pBufferTx, uint8_t *pBufferRx, uint32_t nbBytes, bool useDma)
{
    return BSP_spi2_write_and_read (pBufferTx, pBufferRx, nbBytes, useDma);
}

/*
 * @brief  Register SPI Callback function
 *
 * @param [in] spiCallback - Register SPI IRQ callback function
 * @param [in] hDevice - Pointer to Phy device handler
 * @param [out] none
 * @return none
 *
 * @details
 *
 * @sa
 */
uint32_t HAL_SpiRegisterCallback(HAL_Callback_t const *spiCallback, void * hDevice)
{
    return BSP_spi2_register_callback (spiCallback, hDevice);
}

uint32_t HAL_Init_Hook(void)
{
    return ADI_HAL_SUCCESS;
}

uint32_t HAL_UnInit_Hook(void)
{
    return ADI_HAL_SUCCESS;
}

/** @}*/

