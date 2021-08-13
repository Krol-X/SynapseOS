#define EI_NIDENT 16

struct elf32_hdr {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry; /* Entry point */
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
};

/* e_ident:
    struct {
        unsigned char ei_magic[4];
        unsigned char ei_class;
        unsigned char ei_data;
        unsigned char ei_version;
        unsigned char ei_pad[9];
    }
*/


int LoadELF (unsigned char *bin){
    struct elf32_hdr *EH = (struct elf32_hdr *)bin;
    struct elf32_phdr *EPH;
    if (EH->e_ident[0] != 0x7f || // Контролируем MAGIC
        EH->e_ident[1] != 'E' ||
        EH->e_ident[2] != 'L' ||
        EH->e_ident[3] != 'F' ||
        EH->e_ident[4] != ELFCLASS32 || // Контролируем класс
        EH->e_ident[5] != ELFDATA2LSB || // порядок байт
        EH->e_ident[6] != EV_CURRENT || // версию
        EH->e_type != ET_EXEC || // тип
        EH->e_machine != EM_386 || // платформу
        EH->e_version != EV_CURRENT) // и снова версию, на всякий случай
        return ELF_WRONG;

    EPH = (struct elf32_phdr *)(bin + EH->e_phoff);
    while (EH->e_phnum--) {
        if (EPH->p_type == PT_LOAD)
            memcpy (EPH->p_vaddr, bin + EPH->p_offset, EPH->p_filesz);
            
        EPH = (struct elf32_phdr *)((unsigned char *)EPH + EH->e_phentsize));
    }
    return ELF_OK;
}