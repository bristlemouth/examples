#include "bristlemouth.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0

void app_main(void) {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    bristlemouth_init(NULL);
}
