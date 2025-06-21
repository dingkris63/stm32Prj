ENTRY(Reset_Handler)  /* Entry point (defined in startup file) */

MEMORY
{
    RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);  /* Stack top */

SECTIONS
{
    /* ISR vector table (must be at 0x08000000) */
    .isr_vector :
    {
        *(.isr_vector)
    } >FLASH

    /* Initialized data (.data) */
    _sidata = LOADADDR(.data); /* _sidata: Start of .data in Flash (source of initial values). */
    .data :
    {
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .; /* _edata: End of .data in RAM. */
    } >RAM AT> FLASH
    /*
    >RAM AT> FLASH
    This tells the linker to load the .data section into RAM but store it in Flash.
    The startup code copies it from Flash to RAM during boot.
    */

    /* Uninitialized data (.bss) */
    /*
    Zero-initialized variables (no space in Flash needed).
    The startup code zeros this section (LoopFillZerobss in startup_stm32f407xx.s).
    */
    .bss :
    {
        _sbss = .;
        *(.bss)
        *(.bss*)
        _ebss = .;
    } >RAM

    /* Code and read-only data */
    .text :
    {
        *(.text)
        *(.text*)
    } >FLASH
}