@Echo off

echo boot.bin
start utils/FASM.EXE src/boot.asm bin/boot.bin


echo startup.o
start utils/FASM.EXE src/startup.asm bin/startup.o

echo main.o
gcc -c -m32 -ffreestanding -o bin/main.o src/main.c

echo link 
ld --oformat=binary -melf_i386 -T script.ld -o kernel.bin startup.o main.o

echo Make disk.img
start utils/dd.exe if=bin/boot.bin of=bin/boot_sector.bin bs=512 count=1
start utils/dd.exe if=bin/boot.bin of=disk/boot.bin bs=1 skip=512
start utils/make_listfs.exe of=bin/disk.img bs=512 size=2880 boot=bin/boot_sector.bin src=./disk

pause