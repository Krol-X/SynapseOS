echo boot.bin
FASM src/boot.asm bin/boot.bin
echo startup.o
FASM src/startup.asm bin/startup.o
echo main.o
gcc -c -m32 -ffreestanding -o bin/main.o src/main.c
echo link 
ld --oformat=binary -melf_i386 -T src/script.ld -o kernel.bin bin/startup.o bin/main.o
echo Make disk.img
dd if=bin/boot.bin of=bin/boot_sector.bin bs=512 count=1
dd if=bin/boot.bin of=disk/boot.bin bs=1 skip=512
./utils/make_listfs of=bin/disk.img bs=512 size=2880 boot=bin/boot_sector.bin src=./disk