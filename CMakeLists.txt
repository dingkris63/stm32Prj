cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain paths (adjust if not in PATH)
set(CMAKE_PREFIX_PATH "D:/Tool/ArmToolChain/14.2 rel1/bin")
set(TOOLCHAIN_PREFIX "arm-none-eabi-")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_OBJDUMP      "${TOOLCHAIN_PREFIX}objdump")
set(CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size")

# Compiler/linker flags (optional, can also be in CMakeLists.txt)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb-interwork -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSTM32F40_41xxx -DDUSE_FULL_ASSER")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")  # Debug-specific flags
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

# Disable compiler checks for bare-metal
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(STM32F4 LANGUAGES C CXX ASM)

# Enable compile_commands.json (optional)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -mthumb-interwork # ARM-Thumb interworking (optional, modern GCC handles it automatically)
    -ffunction-sections
    -fdata-sections
    -g
    -O0
)
# Include directories (equivalent to -I in Keil)
include_directories(
    ./CORE
    ./SYSTEM/delay
    ./SYSTEM/sys
    ./SYSTEM/usart
    ./USER
    ./HARDWARE/LED
    ./HARDWARE/BEEP
    ./FWLIB/inc
)

# Macro definitions (equivalent to -D in Keil)
add_definitions(
    -D__UVISION_VERSION=541
    -DSTM32F407xx
    -DSTM32F40_41xxx
    -DUSE_STDPERIPH_DRIVER
)

# Linker flags (Keil's --split_sections ¡ú GCC's garbage collection)
set(CMAKE_EXE_LINKER_FLAGS
    "-Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/STM32F407ZGTx_FLASH.ld"
)
set( SOURCE_FILE
    ./CORE/startup_stm32f407xx.s
    ./FWLIB/src/misc.c
    ./FWLIB/src/stm32f4xx_adc.c
    ./FWLIB/src/stm32f4xx_can.c
    ./FWLIB/src/stm32f4xx_crc.c
    ./FWLIB/src/stm32f4xx_cryp.c
    ./FWLIB/src/stm32f4xx_cryp_aes.c
    ./FWLIB/src/stm32f4xx_cryp_des.c
    ./FWLIB/src/stm32f4xx_cryp_tdes.c
    ./FWLIB/src/stm32f4xx_dac.c
    ./FWLIB/src/stm32f4xx_dbgmcu.c
    ./FWLIB/src/stm32f4xx_dcmi.c
    ./FWLIB/src/stm32f4xx_dma.c
    ./FWLIB/src/stm32f4xx_dma2d.c
    ./FWLIB/src/stm32f4xx_exti.c
    ./FWLIB/src/stm32f4xx_flash.c
    ./FWLIB/src/stm32f4xx_flash_ramfunc.c
    # ./FWLIB/src/stm32f4xx_fmc.c
    ./FWLIB/src/stm32f4xx_fsmc.c
    ./FWLIB/src/stm32f4xx_gpio.c
    ./FWLIB/src/stm32f4xx_hash.c
    ./FWLIB/src/stm32f4xx_hash_md5.c
    ./FWLIB/src/stm32f4xx_hash_sha1.c
    ./FWLIB/src/stm32f4xx_i2c.c
    ./FWLIB/src/stm32f4xx_iwdg.c
    ./FWLIB/src/stm32f4xx_ltdc.c
    ./FWLIB/src/stm32f4xx_pwr.c
    ./FWLIB/src/stm32f4xx_rcc.c
    ./FWLIB/src/stm32f4xx_rng.c
    ./FWLIB/src/stm32f4xx_rtc.c
    ./FWLIB/src/stm32f4xx_sai.c
    ./FWLIB/src/stm32f4xx_sdio.c
    ./FWLIB/src/stm32f4xx_spi.c
    ./FWLIB/src/stm32f4xx_syscfg.c
    ./FWLIB/src/stm32f4xx_tim.c
    ./FWLIB/src/stm32f4xx_usart.c
    ./FWLIB/src/stm32f4xx_wwdg.c
    ./HARDWARE/LED/led.c
    ./main.c
    ./stm32f4xx_it.c
    ./SYSTEM/delay/delay.c
    ./SYSTEM/sys/sys.c
    ./SYSTEM/usart/usart.c
    ./system_stm32f4xx.c
)

add_executable(${PROJECT_NAME}.elf
    ${SOURCE_FILE}
)

target_include_directories(${PROJECT_NAME}.elf PUBLIC
    ${CMAKE_SOURCE_DIR}
    public
    FWLIB/inc
    CORE
    HARDWARE/LED/
    SYSTEM/delay
    SYSTEM/sys
    SYSTEM/usart
)

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -Wall   # Warnings
    -Og     # Optimization for debugging
)

# Generate HEX/BIN files (uses CMAKE_OBJCOPY from toolchain file)
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files"
)
