@Echo off

echo build SynapseOS 0003
echo *************************

SET AS=i686-elf-as
SET CC=i686-elf-gcc
SET LD=i686-elf-ld
SET SRC=./src/kernel
SET BLASTER=A220 I5 D1 P330
ECHO %BLASTER%


SET CCFLAGS = "-std=gnu99 -ffreestanding -Wall -Wextra"
SET LDFLAGS = "-ffreestanding -nostdlib -lgcc"

mkdir bin

echo Build asm kernel
fasm %SRC%/kernel.asm bin/kasm.o

echo %CC% %CCFLAGS% -c %SRC%/kernel.c -o ./bin/kc.o
start %CC% %CCFLAGS% -c %SRC%/kernel.c -o ./bin/kc.o

echo Done
pause