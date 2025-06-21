ENTRY(Reset_Handler)  /* Entry point (defined in startup file) */
/* CPU£º STM32F407ZGT6£¬ LQFP144£¬ FLASH£º 1024K£¬ SRAM£º 192K£» */
MEMORY {
    /* Flash (1MB) */
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 1024K
    /* Main SRAM (112KB) */
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 112K
    /* Optional CCM RAM (64KB, no DMA) */
    CCMRAM (xrw)    : ORIGIN = 0x10000000, LENGTH = 64K
    /* Optional SRAM2 (16KB) */
    SRAM2 (xrw)     : ORIGIN = 0x2001C000, LENGTH = 16K
}

SECTIONS
{
    /* Interrupt vector table at start of Flash */
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Code and read-only data */
    .text : {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Initialized data (.data) */
    _sidata = LOADADDR(.data); /* _sidata: Start of .data in Flash (source of initial values). */
    /* Initialized data (stored in Flash, loaded to RAM at startup) */
    .data : {
        . = ALIGN(4);
        _sdata = .;        /* Start address in RAM */
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* End address in RAM */
    } > RAM AT > FLASH
    /*
    >RAM AT> FLASH
    This tells the linker to load the .data section into RAM but store it in Flash.
    The startup code copies it from Flash to RAM during boot.
    */

    /* Uninitialized data (.bss) */
    /******************************************************************************
    Zero-initialized variables (no space in Flash needed).
    The startup code zeros this section (LoopFillZerobss in startup_stm32f407xx.s).
    ******************************************************************************/
    /* Uninitialized data (zeroed at startup) */
    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* Core-coupled memory (optional) */
    .ccmram : {
        . = ALIGN(4);
        _sccmram = .;
        *(.ccmram*)
        . = ALIGN(4);
        _eccmram = .;
    } > CCMRAM

    /* Stack and heap */
    _estack = ORIGIN(RAM) + LENGTH(RAM); /* Stack grows downward */
    _heap_end = _estack - 16K;           /* Reserve 16KB for stack */

    /* Optional: Place heap in SRAM2 */
    .heap : {
        . = ALIGN(4);
        _sheap = .;
        . = . + (LENGTH(SRAM2) - 4);
        _eheap = .;
    } > SRAM2
}