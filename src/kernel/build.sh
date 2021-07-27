AS="/opt/i686-elf-tools-linux/bin/i686-elf-as"
CC="/opt/i686-elf-tools-linux/bin/i686-elf-gcc"
LD="/opt/i686-elf-tools-linux/bin/i686-elf-ld"

CCFLAGS="-std=gnu99 -ffreestanding -Wall -Wextra"
LDFLAGS="-ffreestanding -nostdlib -lgcc"

mkdir -p bin
rm bin/*
fasm kernel.asm bin/kasm.o
$CC $CCFLAGS -c kernel.c -o bin/kc.o
$CC $CCFLAGS -c Modules/gdt.c -o bin/gdt.o
$CC $CCFLAGS -c Modules/Interrupt_Descriptor_Table.c -o bin/Interrupt_Descriptor_Table.o
$CC $CCFLAGS -c Modules/kbd.c -o bin/kbd.o
$CC $CCFLAGS -c Modules/tty.c -o bin/tty.o
$CC $CCFLAGS -c Modules/ports.c -o bin/ports.o
$CC $CCFLAGS -c Modules/cpu_detect.c -o bin/cpu_detect.o
$CC $CCFLAGS -c Modules/qemu_log.c -o bin/qemu_log.o
$CC $LDFLAGS -T link.ld -o bin/kernel.elf bin/kasm.o bin/kc.o bin/gdt.o bin/Interrupt_Descriptor_Table.o bin/kbd.o bin/tty.o bin/ports.o bin/qemu_log.o bin/cpu_detect.o

rm -rf isodir
mkdir -p isodir/boot/grub
cp bin/kernel.elf isodir/boot/kernel.elf
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o SynapseOS.iso isodir/