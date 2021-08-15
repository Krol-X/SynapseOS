CC="/opt/i686-elf-tools-linux/bin/i686-elf-gcc"

BUILDFLAGS="-std=gnu11 -ffreestanding -Wall -Wextra"
LINKFLAGS="-ffreestanding -nostdlib -lgcc"

cd ..

mkdir -p bin
mkdir -p isodir/boot/grub

# build code into objects
fasm kernel.asm bin/kasm.o
$CC $BUILDFLAGS -c kernel.c -o bin/kc.o
$CC $BUILDFLAGS -c modules/cmos.c -o bin/cmos.o
$CC $BUILDFLAGS -c modules/cpu_detect.c -o bin/cpu_detect.o
$CC $BUILDFLAGS -c modules/gdt.c -o bin/gdt.o
$CC $BUILDFLAGS -c modules/interdesctbl.c -o bin/interdesctbl.o
$CC $BUILDFLAGS -c modules/kbd.c -o bin/kbd.o
$CC $BUILDFLAGS -c modules/ports.c -o bin/ports.o
$CC $BUILDFLAGS -c modules/qemu_log.c -o bin/qemu_log.o
$CC $BUILDFLAGS -c modules/shell.c -o bin/shell.o
$CC $BUILDFLAGS -c modules/string.c -o bin/string.o
$CC $BUILDFLAGS -c modules/tty.c -o bin/tty.o

# link objects
$CC $LINKFLAGS -T link.ld -o isodir/boot/kernel.elf bin/kasm.o bin/kc.o bin/cpu_detect.o bin/gdt.o bin/interdesctbl.o bin/kbd.o bin/ports.o bin/qemu_log.o bin/shell.o bin/string.o bin/tty.o

cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o SynapseOS.iso isodir/
