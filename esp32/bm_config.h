#ifndef __BM_CONFIG_H__
#define __BM_CONFIG_H__

#include <stdio.h>

#define bm_app_name "esp32_bristlemouth"

#define bm_debug(format, ...) printf(format, ##__VA_ARGS__)

#define bm_configuration_size_bytes (5 * 1024)

#endif /* __BM_CONFIG_H__ */
