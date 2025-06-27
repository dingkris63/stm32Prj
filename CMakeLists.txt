cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Toolchain paths (adjust if not in PATH)
set(CMAKE_PREFIX_PATH "D:\\Tool\\ArmToolChain\\14.2 rel1\\bin\\")
set(TOOLCHAIN_PREFIX "arm-none-eabi-")
set(CMAKE_C_COMPILER   "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PREFIX}gcc")
set(CMAKE_OBJCOPY      "${TOOLCHAIN_PREFIX}objcopy")
set(CMAKE_OBJDUMP      "${TOOLCHAIN_PREFIX}objdump")
set(CMAKE_SIZE         "${TOOLCHAIN_PREFIX}size")

# Compiler/linker flags (optional, can also be in CMakeLists.txt)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

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
    ./HARDWARE
    ./HARDWARE/LED
    ./HARDWARE/BEEP
    ./FWLIB/inc
)

# Linker flags (Keil's --split_sections ¡ú GCC's garbage collection)
add_link_options(
    -Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/STM32F407ZGTx_FLASH.ld
    -Wl,-Map=${CMAKE_BINARY_DIR}/sys.map # use arm-none-eabi-nm
    -Wl,--cref # Cross-reference (--xref)
    -Wl,--print-memory-usage # Summary sizes (--info summarysizes)
    -Wl,--gc-sections # Remove unused sections (--info unused)
    -Wl,--no-undefined # Strict linking (no undefined symbols) Equivalent to --strict(ARM KEIL)
    # Below are for C++
    --specs=nano.specs
    --specs=nosys.specs
    -lstdc++
    -lsupc++
)

# Macro definitions (equivalent to -D in Keil)
add_definitions(
    -D__UVISION_VERSION=541
    -DSTM32F407xx
    -DSTM32F40_41xxx
    -D__CORTEX_M4
    -DUSE_STDPERIPH_DRIVER
    -DDUSE_FULL_ASSER
    -D__CORE_CM4_SIMD_H__
)

set( SOURCE_FILE
    ./CORE/startup_stm32f407xx.s
    ./CORE/syscall.c
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
    ./HARDWARE/hwm_init.cpp
    ./HARDWARE/LED/led.c
    ./main.c
    ./stm32f4xx_it.c
    ./system_stm32f4xx.c
)

add_executable(${APP_NAME}.elf
    ${SOURCE_FILE}
)

target_include_directories(${APP_NAME}.elf PRIVATE
    ${CMAKE_SOURCE_DIR}
    public
    FWLIB/inc
    CORE
    HARDWARE/LED
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/OS)
target_link_libraries(${APP_NAME}.elf PRIVATE os_lib)
