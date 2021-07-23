echo build SynapseOS 0001
echo *************************
echo build bootloader
echo boot.bin
./utils/FASM src/boot/boot.asm bin/boot.bin
echo *************************
echo build libs
echo stdlib.o
./utils/i686-elf-gcc -c -m32 -ffreestanding -o bin/stdlib.o src/lib/stdlib.c
echo tty.o
./utils/i686-elf-gcc -c -m32 -ffreestanding -o bin/tty.o src/lib/tty.c
echo *************************
echo build Kernel
echo startup.o
./utils/FASM src/kernel/startup.asm bin/startup.o
echo main.o
./utils/i686-elf-gcc -c -m32 -ffreestanding -o bin/main.o src/kernel/main.c
echo link 
./utils/i686-elf-ld -T src/script.ld -o bin/kernel.bin bin/startup.o bin/stdlib.o bin/tty.o bin/main.o --entry _start
echo *************************
echo Make disk.img
dd if=bin/boot.bin of=bin/boot_sector.bin bs=512 count=1
dd if=bin/boot.bin of=disk/boot.bin bs=1 skip=512
cp bin/kernel.bin disk/kernel.bin
./utils/make_listfs of=disk.img bs=512 size=2880 boot=bin/boot_sector.bin src=./disk 
echo Done