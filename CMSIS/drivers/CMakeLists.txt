add_library(cmsis_driver STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/misc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_adc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_can.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_cryp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_cryp_aes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_cryp_des.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_cryp_tdes.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_dac.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_dbgmcu.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_dcmi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_flash_ramfunc.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_fmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_fsmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_hash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_hash_md5.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_hash_sha1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_ltdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_rng.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_sai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_sdio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_syscfg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/src/stm32f4xx_wwdg.c
)

target_include_directories(cmsis_driver PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx/inc
)