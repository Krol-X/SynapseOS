// PE loader

//Can load PE executable files

/*

struct pe_hdr {
    unsigned long pe_sign;
    unsigned short pe_cputype;
    unsigned short pe_objnum;
    unsigned long pe_time;
    unsigned long pe_cofftbl_off;
    unsigned long pe_cofftbl_size;
    unsigned short pe_nthdr_size;
    unsigned short pe_flags;
    unsigned short pe_magic;
    unsigned short pe_link_ver;
    unsigned long pe_code_size;
    unsigned long pe_idata_size;
    unsigned long pe_udata_size;
    unsigned long pe_entry;
    unsigned long pe_code_base;
    unsigned long pe_data_base;
    unsigned long pe_image_base;
    unsigned long pe_obj_align;
    unsigned long pe_file_align;
    // ... ну и еще много всякого, неважного.
};


struct pe_ohdr {
    unsigned char o_name[8];    //o_name - имя секции, для загрузки абсолютно безразлично;
    unsigned long o_vsize;      //o_vsize - размер секции в памяти;
    unsigned long o_vaddr;      //o_vaddr - адрес в памяти относительно ImageBase;
    unsigned long o_psize;      //o_psize - размер секции в файле;
    unsigned long o_poff;       //o_poff - смещение секции в файле;
    unsigned char o_reserved[12];
    unsigned long o_flags;      //o_flags - флаги секции;
};


int LoadPE (unsigned char *bin)
{
    struct elf32_hdr *PH = (struct pe_hdr *)(bin + *((unsigned long *)&bin[0x3c]));
    // Конечно комбинация не из понятных... просто берем dword по смещению 0x3c
    // И вычисляем адрес PE заголовка в образе файла
    struct elf32_phdr *POH;

    if (PH == NULL || // Контролируем указатель
        PH->pe_sign != 0x4550 || // сигнатура PE {'P', 'E', 0, 0}
        PH->pe_cputype != 0x14c || // i386
        (PH->pe_flags & 2) == 0) // файл нельзя запускать!
        return PE_WRONG;

    POH = (struct pe_ohdr *)((unsigned char *)PH + 0xf8);

    while (PH->pe_obj_num--) {
        if ((POH->p_flags & 0x60) != 0)
        // либо код либо инициализированные данные
        memcpy (PE->pe_image_base + POH->o_vaddr,
            bin + POH->o_poff, POH->o_psize);

        POH = (struct pe_ohdr *)((unsigned char *)POH + sizeof (struct pe_ohdr));
    }
    return PE_OK;
}
*/