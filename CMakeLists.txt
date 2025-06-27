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


# ARM Features Enable
set(USB_DSP OFF)

# Compiler/linker flags (optional, can also be in CMakeLists.txt)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -D__FPU_PRESENT=1 -D__FPU_USED=1 -ffunction-sections -fdata-sections -O0 -g")
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
    # --- Core Architecture ---
    -mcpu=cortex-m4 # Targets Cortex-M4 CPU (Enables M4-specific instructions (including FPU if present))
    -mthumb # # Uses Thumb-2 instruction set (Required for all Cortex-M)
    -mthumb-interwork # ARM-Thumb interworking (optional, modern GCC handles it automatically)

    # --- Preprocessor Macros ---
    -DUSE_STDPERIPH_DRIVER # Uses ST's SPL (not HAL)
    -DSTM32F407xx # # MCU model
    -DSTM32F40_41xxx # # Legacy family define
    -D__CORTEX_M4 # Defines Cortex-M4 core (Needed for CMSIS-Core (core_cm4.h))
    -DUSE_FULL_ASSERT

    # Enables single-precision FPU with 16 double-word registers
    -mfpu=fpv4-sp-d16 #! Mandatory for hardware floating-point operations

    # Uses hardware FPU instructions (not software emulation)
    -mfloat-abi=hard #! Critical for performance (2-10x faster than softfp)

    # Tells CMSIS the MCU has an FPU
    -D__FPU_PRESENT=1 #! Required for CMSIS-Core (core_cm4.h)

    # Ensures the compiler generates FPU instructions
    $<$<COMPILE_LANGUAGE:C>:-D__FPU_PRESENT=1> #! Redundant with -mfloat-abi=hard (can be omitted)

    # # --- Debug/Codegen --
    -ffunction-sections
    -fdata-sections
    -g
    -Og

    # # Below are for C++
    # --specs=nano.specs
    # --specs=nosys.specs
    # -lstdc++
    # -lsupc++
)
# Enable FPU, Enable DSP lib
if(USE_DSP)
    add_compile_options(
        # Enables CMSIS-DSP library optimizations for Cortex-M4
        $<$<COMPILE_LANGUAGE:C>:-DARM_MATH_CM4> #! Required for DSP functions (FFT, filters)
        # Enables SIMD intrinsics (e.g., __SADD8, __QADD)
        -D__CORE_CM4_SIMD_H__ #! Needed for parallel DSP operations
    )
endif()

# Include directories (equivalent to -I in Keil)
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/CMSIS/core/inc
    ${CMAKE_SOURCE_DIR}/CMSIS/system/stm32f4xx
)

# Linker flags (Keil's --split_sections ¡ú GCC's garbage collection)
add_link_options(
    -Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/STM32F407ZGTx_FLASH.ld
    -Wl,-Map=${CMAKE_BINARY_DIR}/sys.map # use arm-none-eabi-nm
    -Wl,--cref # Cross-reference (--xref)
    -Wl,--print-memory-usage # Summary sizes (--info summarysizes)
    -Wl,--gc-sections # Remove unused sections (--info unused)
    -Wl,--no-undefined # Strict linking (no undefined symbols) Equivalent to --strict(ARM KEIL)
)

set( SOURCE_FILE
    ${CMAKE_SOURCE_DIR}/CMSIS/system/stm32f4xx/stm32f4xx_it.c
    ${CMAKE_SOURCE_DIR}/CMSIS/system/stm32f4xx/system_stm32f4xx.c
    ${CMAKE_SOURCE_DIR}/CMSIS/system/stm32f4xx/startup/startup_stm32f407xx.s
    ${CMAKE_SOURCE_DIR}/CMSIS/core/src/syscall.c
    ${CMAKE_SOURCE_DIR}/main.c
)

add_executable(${APP_NAME}.elf
    ${SOURCE_FILE}
)

target_include_directories(${APP_NAME}.elf PRIVATE
    ${CMAKE_SOURCE_DIR}/HARDWARE/LED
)

if(USE_DSP)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/DSP_Lib)
target_link_libraries(${APP_NAME}.elf PRIVATE dsp_lib)
target_link_libraries(${APP_NAME}.elf PRIVATE arm_cortexM4lf_math) # lf' = Little Endian + FPU
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/drivers)
link_libraries(cmsis_driver) # For Peripherals Usage
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/OS)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/HARDWARE)
target_link_libraries(${APP_NAME}.elf PRIVATE os_lib hw_lib)
