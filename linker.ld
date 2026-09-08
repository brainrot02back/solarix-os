ENTRY(protected_mode)

SECTIONS {
    . = 0x10000;
    
    .multiboot ALIGN(0x1000) :
    {
        *(.multiboot)
    }
    
    .text ALIGN(0x1000) :
    {
        *(.text*)
        *(.rodata*)
    }
    
    .data ALIGN(0x1000) :
    {
        *(.data*)
    }
    
    .bss ALIGN(0x1000) :
    {
        *(.bss*)
        *(COMMON)
    }
}
