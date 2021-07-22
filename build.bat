@Echo off

echo Fasm utils/FASM.EXE
start utils/FASM.EXE src/boot.asm bin/boot.bin
start utils/dd.exe if=bin/boot.bios.bin of=bin/boot_sector.bin bs=512 count=1
start utils/dd.exe if=bin/boot.bios.bin of=disk/boot.bin bs=1 skip=512
start utils/make_listfs.exe of=disk.img bs=512 size=2880 boot=bin/boot_sector.bin src=./disk


pause