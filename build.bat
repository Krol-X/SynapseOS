@Echo off

echo build SynapseOS 0001
echo *************************

::Build bootloader
echo build bootloader

echo boot
start utils/FASM.EXE src/boot/boot.asm bin/boot



echo *************************
::Build libs
echo build libs


::Build Interrupts
echo interrupts.o
i686-elf-gcc -c -m32 -ffreestanding -o bin/interrupts.o src/lib/interrupts.c


::Build Stdlib
echo stdlib.o
i686-elf-gcc -c -m32 -ffreestanding -o bin/stdlib.o src/lib/stdlib.c


::Build Text Driver
echo tty.o
i686-elf-gcc -c -m32 -ffreestanding -o bin/tty.o src/lib/tty.c


::Build scancodes
echo scancodes.o
i686-elf-gcc -c -m32 -ffreestanding -o bin/scancodes.o src/lib/scancodes.c


echo *************************
::Build kernel
echo build Kernel

::Build Kernel starter
echo startup.o
start utils/FASM.EXE src/kernel/startup.asm bin/startup.o

::Build Kernel
echo main.o
i686-elf-gcc -c -m32 -ffreestanding -o bin/main.o src/kernel/main.c

::Link kernel with starter and libs
echo link 
i686-elf-ld -T src/script.ld -o bin/kernel bin/startup.o bin/stdlib.o bin/tty.o  bin/interrupts.o bin/scancodes.o bin/main.o --entry _start



echo *************************
::Make img file
echo Make disk.img
start utils/dd.exe if=bin/boot of=bin/boot_sector bs=512 count=1
start utils/dd.exe if=bin/boot of=disk/boot bs=1 skip=512
cp bin/kernel disk/kernel.bin
start utils/make_listfs.exe of=disk.img bs=512 size=2880 boot=bin/boot_sector src=./disk 




echo *************************
::Clean trash


echo *************************
echo Done
pause