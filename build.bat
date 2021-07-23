@Echo off

echo boot.bin
start utils/FASM.EXE src/boot/boot.asm bin/boot.bin


echo startup.o
start utils/FASM.EXE src/kernel/startup.asm bin/startup.o

echo main.o
i686-elf-gcc -c -m32 -ffreestanding -o bin/main.o src/kernel/main.c

echo link 
i686-elf-ld -T src/script.ld -o bin/kernel.bin bin/startup.o bin/main.o --entry _start

echo Make disk.img
start utils/dd.exe if=bin/boot.bin of=bin/boot_sector.bin bs=512 count=1
start utils/dd.exe if=bin/boot.bin of=disk/boot.bin bs=1 skip=512
cp bin/kernel.bin disk/kernel.bin
start utils/make_listfs.exe of=disk.img bs=512 size=2880 boot=bin/boot_sector.bin src=./disk 

pause