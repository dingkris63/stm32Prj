## ARM TOOL CHAIN
The ARM GNU Toolchain 14.2.Rel1 is a recent version released in October 2024. The best way to get the correct version for your system is from the official ARM website.

Primary Download Source (Official ARM Website)
The most reliable source is ARM's own developer portal:

ARM Developer Downloads Page (Choose your OS and target):
https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

On this page, you need to select:

"Toolchain": ARM GNU Toolchain

"Version": 14.2.Rel1

"Architecture": Choose your target architecture. The most common choices are:

AArch32 bare-metal target (arm-none-eabi) for Cortex-M/R microcontrollers.

AArch64 bare-metal target (aarch64-none-elf) for Cortex-A application processors.

AArch64 Linux target (aarch64-none-linux-gnu) for building Linux applications.

"Host Platform": Select your operating system (Windows, Linux, macOS).

Direct Download Links (For Reference)
Below are direct links for the most common configurations. It is highly recommended to use the official page above, as ARM occasionally changes link structures.

For Windows (x86_64)
AArch32 Bare-metal (arm-none-eabi):
https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-mingw-w64-i686-arm-none-eabi.zip

AArch64 Bare-metal (aarch64-none-elf):
https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-mingw-w64-i686-aarch64-none-elf.zip

For Linux (x86_64)
AArch32 Bare-metal (arm-none-eabi):
https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz

AArch64 Bare-metal (aarch64-none-elf):
https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-aarch64-none-elf.tar.xz

For macOS (Apple Silicon / x86_64)
AArch32 Bare-metal (arm-none-eabi):
https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-darwin-x86_64-arm-none-eabi.tar.xz

AArch64 Bare-metal (aarch64-none-elf):
https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-darwin-x86_64-aarch64-none-elf.tar.xz

Brief Installation & Setup Guide
Download: Get the correct archive for your system.

Extract: Unpack the archive to a directory of your choice. For example:

Windows: C:\Tools\arm-gnu-toolchain\

Linux/macOS: ~/opt/ or /usr/local/

Add to System PATH: This is crucial for your system to find the tools.

Add the bin folder from your extracted directory to your system's PATH environment variable.

Example: If you extracted to C:\Tools\arm-gnu-toolchain\, add C:\Tools\arm-gnu-toolchain\bin to your PATH.

Verify Installation: Open a new terminal or command prompt and run:

bash
arm-none-eabi-gcc --version
# or
aarch64-none-elf-gcc --version
A successful installation will output version information, confirming the toolchain is ready to use:
arm-none-eabi-gcc (Arm GNU Toolchain 14.2.Rel1) 14.2.1 20241010

## ARM GCC vs. GNU GCC

ARM GCC lacks of some syscall, and need to implement it  by users.

## gcc-competible startup arm file

https://github.com/STMicroelectronics/cmsis-device-f4.git: Source/Templates/gcc/startup_stm32f407xx.s

## elf/bin/hex

## issue #1 from link file

### isr_vector

### **Explanation of the Section**

```LD
.isr_vector :
{
  . = ALIGN(4);          /* Align to 4 bytes (Cortex-M requires 32-bit aligned vectors) */
  KEEP(*(.isr_vector))   /* Force linker to keep this section, even if unused */
  . = ALIGN(4);          /* Re-align after the section */
} >FLASH                 /* Place this section in FLASH memory (starting at 0x08000000) */
```

------

### **Key Components**

1. **`.isr_vector`**
   - This is the **interrupt vector table**, a critical part of Cortex-M firmware.
   - It contains addresses of exception handlers (e.g., `Reset_Handler`, `SysTick_Handler`, etc.).
   - The STM32 boots by loading the first entry (`Reset_Handler`) from this table.
2. **Alignment (`. = ALIGN(4)`)**
   - Ensures the vector table starts at a **4-byte aligned address** (Cortex-M4 requires this for correct exception handling).
   - Misalignment can cause hard faults.
3. **`KEEP(*(.isr_vector))`**
   - `KEEP` prevents the linker from discarding this section (even if itâ€™s not referenced in code).
   - `*(.isr_vector)` collects all input object files' `.isr_vector` sections into this output section.
4. **`>FLASH`**
   - Places the vector table in **Flash memory** (address `0x08000000`), where the STM32 boots from.

------

### **Why This Matters**

- **Boot Process**: The STM32 reads the first word of `.isr_vector` (usually the initial stack pointer) and the second word (the `Reset_Handler` address) on startup.
- **Hard Faults if Missing**: If this section is omitted or misaligned, the MCU may fail to boot or crash immediately.
- **Linker Script vs. Startup File**:
  - The linker script (above) places the table in Flash.
  - The **startup file** (e.g., `startup_stm32f407xx.s`) defines the actual vector table content (e.g., `DCD Reset_Handler`).

------

### **Example Vector Table (from Startup File)**

```assembly
.section  .isr_vector, "a"  ; "a" for allocatable section
.align 2                    ; Align to 4 bytes (2^2)
.global  __isr_vector
__isr_vector:
  DCD  _estack              ; First entry: initial stack pointer
  DCD  Reset_Handler        ; Second entry: Reset handler
  DCD  NMI_Handler          ; NMI handler
  DCD  HardFault_Handler    ; Hard fault handler
  ...                       ; Other exceptions
```

------

### **Common Pitfalls**

1. **Incorrect Placement**
   If `.isr_vector` is not at `0x08000000`, the MCU wonâ€™t boot. Verify with:

   ```
   arm-none-eabi-objdump -h firmware.elf | grep isr_vector
   ```

   Output should show the section at `0x08000000`.

2. **Missing `KEEP`**
   If the linker discards `.isr_vector` (e.g., due to `-gc-sections`), the firmware will fail.

3. **Misalignment**
   Cortex-M4 requires the table to be 128-byte aligned (for SCB->VTOR). Modern linker scripts handle this, but older ones may need manual alignment.

------

### **How to Debug Issues**

1. **Check ELF Sections**

   ```
   arm-none-eabi-readelf -S firmware.elf
   ```

   Ensure `.isr_vector` exists and is in Flash.

2. **Verify Vector Addresses**

   ```
   arm-none-eabi-objdump -D firmware.elf | grep -A50 "isr_vector"
   ```

   The first two entries should match `_estack` and `Reset_Handler`.

3. **Linker Map File**
   Build with `-Wl,-Map=firmware.map` and check:

   ```sh
   .isr_vector      0x08000000      0x400
   ```

------

### **Summary**

- This linker script snippet ensures the interrupt vector table is:
  - **Retained** (`KEEP`).
    **Aligned** (4-byte).
    **Placed at Flash start** (`>FLASH`).
- Critical for successful booting. Let me know if you need further clarification!

### text

#### **1. Section Structure**

```
.text :
{
  . = ALIGN(4);        /* Ensure 4-byte alignment (Cortex-M requirement) */
  *(.text)             /* All '.text' sections from input files */
  *(.text*)            /* All '.text*' sections (e.g., .text.function_name) */
  *(.glue_7)           /* ARM-Thumb interworking glue code (ARM->Thumb) */
  *(.glue_7t)          /* ARM-Thumb interworking glue code (Thumb->ARM) */
  *(.eh_frame)         /* Exception handling frames (unwinding info) */

  KEEP (*(.init))      /* Force keeping startup initialization code */
  KEEP (*(.fini))      /* Force keeping termination code */

  . = ALIGN(4);        /* Re-align after the section */
  _etext = .;          /* Define a global symbol marking the end of .text */
} >FLASH               /* Place this section in FLASH memory */
```

------

#### **2. Key Components Explained**

##### **a. `.text` and `.text\*`**

- Collects **all executable code** from compiled object files (`.o`).
- `*(.text)` matches the standard `.text` section (e.g., `main()`).
- `*(.text*)` catches compiler-generated sections like `.text.function_name`.

##### **b. ARM-Thumb Glue Code (`.glue_7`, `.glue_7t`)**

- The Cortex-M4 uses **Thumb-2 instruction set**, but some legacy code may mix ARM/Thumb.
- These sections contain **interworking glue code** generated by the linker to switch between ARM/Thumb modes (rarely needed on Cortex-M4 but kept for compatibility).

#### **c. Exception Handling (`.eh_frame`)**

- Contains **unwinding info** for C++ exceptions or debuggers.
- Typically small (or empty) in bare-metal embedded projects.

#### **d. `KEEP` Directives**

- `KEEP(*(.init))` and `KEEP(*(.fini))` ensure:
  - **Startup code** (e.g., `_start`) is not discarded.
  - **Static destructors** (for C++) are retained (if applicable).

#### **e. `_etext` Symbol**

- Marks the **end of the `.text` section** in Flash.
- Used by the startup code to know where initialized data (`.data`) begins in Flash.

#### **f. `>FLASH`**

- Places the `.text` section in **Flash memory** (address `0x08000000 + size of `.isr_vector`).
- Code executes directly from Flash (no need to copy to RAM).

------

#### **3. Why This Matters**

- **Code Placement**: Ensures all firmware code is stored in Flash.
- **Alignment**: Cortex-M4 requires 4-byte alignment for instructions. Misalignment causes faults.
- **Size Optimization**: The linker can discard unused functions (if `-gc-sections` is enabled), but `KEEP` preserves critical code.
- **Startup Compatibility**: `_etext` is used by `startup_stm32f4xx.s` to locate `.data` and `.rodata`.

------

#### **4. Common Pitfalls**

#### **a. Missing `KEEP` for Critical Code**

If `.init`/`.fini` are discarded (e.g., due to aggressive garbage collection), the firmware may:

- Fail to initialize static variables.
- Crash before `main()`.

#### **b. Incorrect Alignment**

Unaligned code sections can trigger **hard faults** or performance penalties.

#### **c. Glue Code Overhead**

Unnecessary `.glue_7` sections bloat the binary if mixing ARM/Thumb modes (unlikely on Cortex-M4).

------

#### **5. Debugging Tips**

#### **Check Section Placement**

```
arm-none-eabi-objdump -h firmware.elf | grep .text
```

Output should show `.text` in Flash (e.g., `0x080001c0` after `.isr_vector`).

#### **Verify `_etext` Symbol**

```
arm-none-eabi-nm firmware.elf | grep _etext
```

This address should match the end of `.text` and the start of `.data` in Flash.

#### **Map File Analysis**

Build with `-Wl,-Map=firmware.map` and check:

```
.text           0x080001c0     0x1234
*(.text)
*(.text*)
...
_etext = 0x080014f4
```

------

#### **6. Practical Implications**

- If your firmware crashes before `main()`:
  - Check if `.init` or `.isr_vector` was discarded.
  - Verify alignment in the linker script.
- If the binary is too large:
  - Remove unused glue code (ensure pure Thumb-2 compilation with `-mthumb`).
  - Use `-ffunction-sections` and `-gc-sections` in GCC to discard unused functions.

------

#### **Summary**

This `.text` section definition:
âœ? Ensures code is placed in Flash.
âœ? Retains critical startup/termination code.
âœ? Provides alignment and symbols for the startup script.
âœ? Handles ARM/Thumb interworking (if needed).

##  .rodata

```
.rodata :
{
  . = ALIGN(4);          /* Align current location to 4 bytes */
  *(.rodata)             /* Exact match for .rodata sections */
  *(.rodata*)            /* Wildcard match for all .rodata.* sections */
  . = ALIGN(4);          /* Align again after section contents */
} >FLASH                 /* Place entire section in FLASH memory */
```

------

### **Key Components Explained**

1. **`. = ALIGN(4);`**
   - Ensures the section starts on a **4-byte boundary** (ARM requires word alignment for efficient access).
   - The dot (`.`) represents the **current location counter**.
2. **`\*(.rodata)`**
   - Matches **only sections named exactly `.rodata`** (no suffixes).
   - Contains:
     - String literals (`"Hello World"`)
     - Global `const` variables
     - Compiler-generated constants
3. **`\*(.rodata\*)`**
   - Wildcard match for **all variants** of `.rodata`, including:
     - `.rodata.str1.4` (compiler-optimized strings)
     - `.rodata.cst4` (4-byte constants)
     - Vendor-specific sections (e.g., `.rodata.foo`)
4. **Final `ALIGN(4)`**
   - Pads the section to a 4-byte boundary after all `.rodata` content.
   - Prevents misalignment issues when the next section starts.
5. **`>FLASH`**
   - Directs the linker to place `.rodata` in **FLASH memory** (typical for read-only data).

------

### **Why This Structure Matters**

#### **1. Correctness**

- **Alignment** avoids CPU faults (e.g., Cortex-M requires aligned access for `LDR` instructions).
- **Wildcards** ensure no read-only data is accidentally omitted.

#### **2. Optimization**

- Compilers (GCC/Clang) generate specialized `.rodata.*` sections for:
  - Strings with different alignment/packing (`str1.1`, `str1.4`)
  - Constants grouped by size (`cst4`, `cst8`)
- The wildcard `*(.rodata*)` captures all variants.

#### **3. Debugging**

If your program crashes when accessing constants:

1. Check alignment:

   ```
   arm-none-eabi-objdump -h firmware.elf | grep rodata
   ```

   Output should show addresses divisible by 4 (e.g., `0800A000`).

2. Verify content:

   ```
   arm-none-eabi-objdump -s -j .rodata firmware.elf
   ```

------

### **Common Pitfalls**

1. **Missing `ALIGN`**
   â†? May cause **hard faults** on strict-alignment architectures (Cortex-M0/M3).
2. **Omitting `\*(.rodata\*)`**
   â†? **Lost constants** (compiler-generated `.rodata.*` sections wonâ€™t be included).
3. **Incorrect Memory Region**
   â†? Placing `.rodata` in RAM wastes space (it canâ€™t be modified anyway).

------

### **Advanced Usage**

#### **Merge All Read-Only Sections**

```
.rodata : {
  *(.rodata .rodata.* .gnu.linkonce.r.*)
} >FLASH
```

#### **Control Ordering**

```
.rodata : {
  *(.rodata.sort_me_first)  /* Specific section first */
  *(.rodata*)
} >FLASH
```

#### **Compression (Advanced)**

Some systems compress `.rodata` to save FLASH (decompress at startup):

```
.rodata.compressed : {
  *(.rodata*)
} >FLASH
.rodata : {
  *(.rodata_decompress_buffer)
} >RAM
```

------

### **Summary**

This `.rodata` definition:

1. **Aligns** properly for ARM.
2. **Captures all read-only data** (exact + wildcard).
3. **Places in FLASH** (correct memory region).
4. **Follows best practices** for embedded systems.

Always include both `*(.rodata)` and `*(.rodata*)` unless you have a specific reason to exclude certain sections.
