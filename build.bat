@Echo off
echo build SynapseOS 0003
echo *************************




SET AS=i686-elf-as
SET CC=i686-elf-gcc
SET LD=i686-elf-ld
SET SRC=./src/kernel
SET CCFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra
SET LDFLAGS=-ffreestanding -nostdlib -lgcc




mkdir bin & mkdir isodir & cd isodir & mkdir boot & cd boot & mkdir grub & cd ../../




echo Build asm kernel
fasm %SRC%/kernel.asm bin/kasm.o




echo Build kernel

echo %CC% %CCFLAGS% -c %SRC%/kernel.c -o ./bin/kc.o
%CC% %CCFLAGS% -c %SRC%/kernel.c -o ./bin/kc.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/gdt.c -o bin/gdt.o
%CC% %CCFLAGS% -c %SRC%/Modules/gdt.c -o bin/gdt.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/interdesctbl.c -o bin/interdesctbl.o
%CC% %CCFLAGS% -c %SRC%/Modules/interdesctbl.c -o bin/interdesctbl.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/kbd.c -o bin/kbd.o
%CC% %CCFLAGS% -c %SRC%/Modules/kbd.c -o bin/kbd.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/tty.c -o bin/tty.o
%CC% %CCFLAGS% -c %SRC%/Modules/tty.c -o bin/tty.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/ports.c -o bin/ports.o
%CC% %CCFLAGS% -c %SRC%/Modules/ports.c -o bin/ports.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/cpu_detect.c -o bin/cpu_detect.o
%CC% %CCFLAGS% -c %SRC%/Modules/cpu_detect.c -o bin/cpu_detect.o

echo %CC% %CCFLAGS% -c %SRC%/Modules/qemu_log.c -o bin/qemu_log.o
%CC% %CCFLAGS% -c %SRC%/Modules/qemu_log.c -o bin/qemu_log.o

echo %CC% %CCFLAGS% -c %SRC%/link.ld -o bin/kernel.elf bin/kasm.o bin/kc.o bin/gdt.o bin/interdesctbl.o bin/kbd.o bin/tty.o bin/ports.o bin/qemu_log.o bin/cpu_detect.o
%CC% %LDFLAGS% -T %SRC%/link.ld -o bin/kernel.elf bin/kasm.o bin/kc.o bin/gdt.o bin/interdesctbl.o bin/kbd.o bin/tty.o bin/ports.o bin/qemu_log.o bin/cpu_detect.o




echo Create iso
cp bin/kernel.elf isodir/boot/kernel.elf
cp %SRC%/grub.cfg isodir/boot/grub/grub.cfg
ubuntu run grub-mkrescue -o SynapseOS.iso isodir/



echo Done
pause