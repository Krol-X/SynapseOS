OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    /* Base address starting at 1MB */
    . = 0x00100000;
    
    /* Special multiboot compliant header.
     * Used by GRUB for image detection.
    */

    .__mbHeader : { 
        *(.__mbHeader) 
    }

    .early_text ALIGN (0x1000) : { 
        *(.early_text) 
    }

    .early_data ALIGN (0x1000) : { 
        *(.early_data) 
    }

    .early_bss ALIGN (0x1000) :{
        _EARLY_BSS_START = .;
        *(.early_bss)
        _EARLY_BSS_END = .;
    }

    .text : { 
        *(.text) 
    }

    .data : { 
        *(.data)
    }

    .bss  : { 
        *(.bss)  
    }
}