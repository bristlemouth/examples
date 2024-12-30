#include "bm_configs_generic.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define PARTITION_NAMESPACE "bm_config"

bool bm_config_read(BmConfigPartition partition, uint32_t offset, uint8_t *buffer,
                    size_t length, uint32_t timeout_ms) {
  (void)offset;
  (void)timeout_ms;
  bool ret = false;
  size_t length_temp = length;
  const char *partition_s = NULL;
  esp_err_t err;
  nvs_handle_t nvs_handle;

  err = nvs_open(PARTITION_NAMESPACE, NVS_READWRITE, &nvs_handle);

  if (err == ESP_OK) {
    switch (partition) {
    case BM_CFG_PARTITION_USER:
      partition_s = "user";
      break;
    case BM_CFG_PARTITION_SYSTEM:
      partition_s = "sys";
      break;
    case BM_CFG_PARTITION_HARDWARE:
      partition_s = "hw";
      break;
    default:
      break;
    }
    err = nvs_get_blob(nvs_handle, (const char *)partition_s, (void *)buffer,
                       &length_temp);

    if (err == ESP_OK) {
        ret = true;
    }
  }

  return ret;
}

bool bm_config_write(BmConfigPartition partition, uint32_t offset, uint8_t *buffer,
                     size_t length, uint32_t timeout_ms) {
  (void)offset;
  (void)timeout_ms;
  bool ret = false;
  const char *partition_s = NULL;
  esp_err_t err;
  nvs_handle_t nvs_handle;

  err = nvs_open(PARTITION_NAMESPACE, NVS_READWRITE, &nvs_handle);

  if (err == ESP_OK) {
    switch (partition) {
    case BM_CFG_PARTITION_USER:
      partition_s = "user";
      break;
    case BM_CFG_PARTITION_SYSTEM:
      partition_s = "sys";
      break;
    case BM_CFG_PARTITION_HARDWARE:
      partition_s = "hw";
      break;
    default:
      break;
    }
    err = nvs_set_blob(nvs_handle, (const char *)partition_s, (void *)buffer,
                       length);
    if (err == ESP_OK) {
        ret = true;
    }
  }

  return ret;
}

void bm_config_reset(void) { esp_restart(); }
