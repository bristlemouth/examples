cmake_minimum_required(VERSION 3.17)

message(STATUS "Build Type: ${BUILD_TYPE}")
project(bristlemouth_esp32 C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)
#idf_build_component(components/esp-aws-iot)
idf_build_process("esp32"
                  COMPONENTS freertos
                             esptool_py
                             bt
                             nvs_flash
                             driver
                             esp_https_ota
                             esp-tls
                             esp_timer
                             json
                             mbedtls
                             log
                             vfs
                             app_update
                             mqtt
                             wifi_provisioning
                             #esp-aws-iot
                  SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
                  BUILD_DIR ${CMAKE_BINARY_DIR})

set(BM_CORE_FREERTOS_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/config/include/freertos
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/esp_additions/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/config/${arch}/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/config/${arch}/include/freertos
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/FreeRTOS-Kernel/include/freertos
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/FreeRTOS-Kernel/portable/${arch}
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/FreeRTOS-Kernel/portable/${arch}/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/freertos/FreeRTOS-Kernel/portable/${arch}/include/freertos
    ${CMAKE_BINARY_DIR}/config
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/xtensa/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/xtensa/${target}/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/esp_hw_support/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/soc/${target}/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/esp_common/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/esp_system/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/newlib/platform_include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/heap/include
    ${CMAKE_CURRENT_LIST_DIR}/esp-idf/components/esp_rom/include
)

set(LWIP_DIR ${CMAKE_CURRENT_LIST_DIR}/lwip)
set(LWIP_INCLUDE_DIRS
    ${BM_CORE_FREERTOS_INCLUDES}
    ${LWIP_DIR}/src/include
    ${LWIP_DIR}/contrib/ports/freertos/include
    ${CMAKE_BINARY_DIR}/config
    ${CMAKE_CURRENT_LIST_DIR}
)

# Add project wide includes
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/.
    ${LWIP_INCLUDE_DIRS}
    ${BM_CORE_FREERTOS_INCLUDES}
)
add_compile_definitions(ESP_PLATFORM=1)


# Add lwip
include(${LWIP_DIR}/src/Filelists.cmake)

# Add bm_core
set(BM_CORE_LWIP_INCLUDES
    ${LWIP_INCLUDE_DIRS}
)

include(bm_core/cmake/bm_core.cmake)
setup_bm_ip_stack(LWIP "${BM_CORE_LWIP_INCLUDES}")
setup_bm_os(FREERTOS "${BM_CORE_FREERTOS_INCLUDES}")
add_subdirectory(bm_core bmcore)
target_compile_options(bmcore PRIVATE
    -std=gnu17
)
target_link_libraries(bmcore PUBLIC lwipcore)

# Add sources to executable
add_executable(${CMAKE_PROJECT_NAME}.elf
    arch/sys_arch.c
    lwip_support.c
    port/adi_hal.c
    port/rtc.c
    port/config.c
    port/dfu.c
    main.c)

target_link_libraries(${CMAKE_PROJECT_NAME}.elf
                      idf::newlib
                      idf::freertos
                      idf::spi_flash
                      idf::bt
                      idf::nvs_flash
                      idf::driver
                      idf::esp_https_ota
                      idf::json
                      idf::mbedtls
                      idf::log
                      idf::app_update
                      idf::mqtt
                      idf::wifi_provisioning
                      #idf::esp-aws-iot
                      bmcommon
                      bcmp
                      middleware
                      bmmessages
                      bmnetwork
                      bmadin2111
                      bmthirdparty
                      bmintegrations
                      bmcore
                      lwipcore
)

idf_build_executable(${CMAKE_PROJECT_NAME}.elf)

#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/aws-root-ca.pem" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/certificate.pem.crt" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/server.cert" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/device.cert" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/private.pem.key" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/deviceid.txt" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/endpoint.txt" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "${CMAKE_CURRENT_LIST_DIR}/github_server.cert" TEXT)
