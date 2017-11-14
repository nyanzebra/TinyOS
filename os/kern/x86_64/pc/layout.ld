ENTRY(start)

SECTIONS {
  . = 1M;

	.multiboot_header :
	{
		/* ensure that the multiboot header is at the beginning */
		KEEP(*(.multiboot_header))
	}

  .text BLOCK(4K) : ALIGN(4K)
  {
    *(.text .text.*)
  }

  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata .rodata.*)
  }

  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data .data.*)
  }

  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss .bss.*)
  }
}
