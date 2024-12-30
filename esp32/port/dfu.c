#include "messages.h"
#include "bm_dfu_generic.h"

BmErr bm_dfu_client_set_confirmed(void) {
  return BmOK;
}

BmErr bm_dfu_client_set_pending_and_reset(void) {
  return BmOK;
}

BmErr bm_dfu_client_fail_update_and_reset(void) {
  return BmOK;
}

BmErr bm_dfu_client_flash_area_open(const void **flash_area) {
  return BmOK;
}

BmErr bm_dfu_client_flash_area_close(const void *flash_area) {
  return BmOK;
}

BmErr bm_dfu_client_flash_area_write(const void *flash_area, uint32_t off, const void *src,
                                     uint32_t len) {
  return BmOK;
}

BmErr bm_dfu_client_flash_area_erase(const void *flash_area, uint32_t off, uint32_t len) {
  return BmOK;
}

uint32_t bm_dfu_client_flash_area_get_size(const void *flash_area) {
  return BmOK;
}

BmErr bm_dfu_host_get_chunk(uint32_t offset, uint8_t *buffer, size_t len, uint32_t timeout_ms) {
  return BmOK;
}

void bm_dfu_core_lpm_peripheral_active(void) { }

void bm_dfu_core_lpm_peripheral_inactive(void) { }
