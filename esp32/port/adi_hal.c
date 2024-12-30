#include "adi_hal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "l2.h"
#include <stdio.h>
#include <string.h>

#define SPI_HOST_ADIN    SPI2_HOST
#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_INT 5

#define SPI_TASK_SIZE_BYTES 4096
#define SPI_TASK_SIZE_PRIORITY 15

struct AdinCtx {
    PRIVILEGED_DATA portMUX_TYPE adin_lock;
    spi_device_handle_t spi;
    SemaphoreHandle_t cb_semaphore;
    SemaphoreHandle_t transaction_semaphore;
};
static HAL_Callback_t ADIN2111_MAC_SPI_CALLBACK = NULL;
static void *ADIN2111_MAC_SPI_CALLBACK_PARAM = NULL;
static struct AdinCtx CTX;

static void IRAM_ATTR adin_isr_handler(void* arg)
{
    bm_l2_handle_device_interrupt();
}

static void spi_pre_transfer_callback(spi_transaction_t *t) {
}

static void spi_post_transfer_callback(spi_transaction_t *t) {
    static BaseType_t xHigherPriorityTaskWokenTransaction;
    static BaseType_t xHigherPriorityTaskWokenCb;
    xHigherPriorityTaskWokenTransaction = pdFALSE;
    xHigherPriorityTaskWokenCb = pdFALSE;
    xSemaphoreGiveFromISR(CTX.cb_semaphore, &xHigherPriorityTaskWokenCb);
    xSemaphoreGiveFromISR(CTX.transaction_semaphore, &xHigherPriorityTaskWokenTransaction);

    if (xHigherPriorityTaskWokenCb == pdTRUE ||
        xHigherPriorityTaskWokenTransaction == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

static void spi_task(void *args) {
    printf("Starting SPI Task...\n");
    while (1) {
        if (xSemaphoreTake(CTX.cb_semaphore, portMAX_DELAY) == pdTRUE) {
            if (ADIN2111_MAC_SPI_CALLBACK) {
                ADIN2111_MAC_SPI_CALLBACK(ADIN2111_MAC_SPI_CALLBACK_PARAM, 0, NULL);
            }
        }
    }
}

uint32_t HAL_EnterCriticalSection(void) {
  taskENTER_CRITICAL(&CTX.adin_lock);
  return 0;
}

uint32_t HAL_ExitCriticalSection(void) {
  taskEXIT_CRITICAL(&CTX.adin_lock);
  return 0;
}

uint32_t HAL_DisableIrq(void) {
    gpio_intr_disable(PIN_INT);
    return ADI_HAL_SUCCESS;
}

uint32_t HAL_EnableIrq(void) {
    gpio_intr_enable(PIN_INT);
    return ADI_HAL_SUCCESS;
}

uint32_t HAL_GetPendingIrq(void) { return 0; }

uint32_t HAL_GetEnableIrq(void) { return 0; }

uint32_t HAL_Init_Hook(void) {
    esp_err_t ret;
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
    };
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 10 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 7,
        .pre_cb = spi_pre_transfer_callback,
        .post_cb = spi_post_transfer_callback,
    };
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .pin_bit_mask = 1 << PIN_INT,
        .mode = GPIO_MODE_INPUT,
    };

    CTX.adin_lock = (portMUX_TYPE)portMUX_INITIALIZER_UNLOCKED;
    CTX.spi = NULL;

    //Initialize Interrupt Pin
    gpio_isr_handler_add(PIN_INT, adin_isr_handler, NULL);

    //Initialize the SPI bus
    ret = spi_bus_initialize(SPI_HOST_ADIN, &buscfg, SPI_DMA_CH_AUTO);
    if (ret == ESP_OK) {
        ret = spi_bus_add_device(SPI_HOST_ADIN, &devcfg, &CTX.spi);
    }

    // Create Semaphores
    CTX.cb_semaphore = xSemaphoreCreateBinary();
    if (CTX.cb_semaphore) {
        CTX.transaction_semaphore = xSemaphoreCreateBinary();
    }
    ret = CTX.cb_semaphore != NULL && CTX.transaction_semaphore != NULL ?
        ESP_OK : ESP_ERR_NO_MEM;

    // Start SPI Task
    if (ret == ESP_OK) {
        ret = xTaskCreate(spi_task, "spi_task", SPI_TASK_SIZE_BYTES, NULL,
                SPI_TASK_SIZE_PRIORITY, NULL) == pdPASS ? ESP_OK : ESP_ERR_NO_MEM;
    }

    return ret == ESP_OK ? ADI_HAL_SUCCESS : ADI_HAL_ERROR;
}

uint32_t HAL_UnInit_Hook(void) { return ADI_HAL_SUCCESS; }

// Required by the Analog Devices adi_hal.h to be implementeed by the application.
// The user will implement the bm_network_spi_read_write function in their application.
uint32_t HAL_SpiReadWrite(uint8_t *pBufferTx, uint8_t *pBufferRx, uint32_t nBytes,
                          bool useDma) {
    esp_err_t ret = ESP_ERR_TIMEOUT;
    spi_transaction_t t = {0};

    if (xSemaphoreTake(CTX.transaction_semaphore, portMAX_DELAY) == pdTRUE) {
        t.tx_buffer = pBufferTx;
        t.rx_buffer = pBufferRx;
        // Transaction length is set in bits
        t.length = nBytes * 8;
        t.rxlength = t.length;
        // Flags must be set to 0 to ensure buffer pointers are used for tx and rx
        t.flags = 0;

        if (useDma) {
            ret = spi_device_queue_trans(CTX.spi, &t, portMAX_DELAY);
        } else {
            ret = spi_device_polling_transmit(CTX.spi, &t);
        }
    }

    return ret == ESP_OK ? ADI_HAL_SUCCESS : ADI_HAL_ERROR;
}

// Required by the Analog Devices adi_hal.h to be implementeed by the application.
// We save the pointer here in our driver wrapper to simplify Bristlemouth integration.
uint32_t HAL_SpiRegisterCallback(HAL_Callback_t const *spiCallback, void *hDevice) {
  // Analog Devices code has a bug at adi_mac.c:535 where they
  // cast a function pointer to a function pointer pointer incorrectly.
  ADIN2111_MAC_SPI_CALLBACK = (const HAL_Callback_t)spiCallback;
  ADIN2111_MAC_SPI_CALLBACK_PARAM = hDevice;
  return ADI_ETH_SUCCESS;
}
