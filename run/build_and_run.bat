::
:: Build and run script
::

@Echo off
set VERSION="0.8.2"

echo build SynapseOS %VERSION%
cd ..
echo *************************
:: Creating a Newline variable (the two blank lines are required!)
set NLM=^


set NL=^^^%NLM%%NLM%^%NLM%%NLM%
IF EXIST "./src/include/kernel.h" (
    set KERNELH_EXIST=1
) ELSE (
    set KERNELH_EXIST=0
   call :GetUnixTime UNIX_TIME
)


:vars
IF EXIST "./src/include/kernel.h" (
    set KERNELH_EXIST=1
) ELSE (
    set KERNELH_EXIST=0
   echo #ifndef KERNEL_CONFIG %NL%#define KERNEL_CONFIG %NL%#define VERSION %VERSION% %NL%#define BUILD_UID "%UNIX_TIME%" %NL%#endif>src/include/kernel.h
)
SET AS=i686-elf-as
SET CC=i686-elf-gcc
SET LD=i686-elf-ld
SET SRC=./src
SET CCFLAGS=-O3 -std=gnu99 -ffreestanding -Wall -Wextra 
SET LDFLAGS=-O3 -ffreestanding -nostdlib -lgcc
set OBJECTS=bin/kasm.o bin/kc.o bin/gdt.o bin/cmos.o bin/time.o bin/vga.o bin/shell.o bin/idt.o bin/kbd.o bin/tty.o bin/ports.o bin/qemu_log.o bin/cpu_detect.o bin/memory_manager.o bin/stdlib.o


IF EXIST "./bin/" (
    echo Cleaning bin folder
) ELSE (
    mkdir bin & mkdir isodir & cd isodir & mkdir boot & cd boot & mkdir grub & cd ../../
)


echo Build asm kernel
fasm %SRC%/kernel.asm bin/kasm.o



echo Build kernel
%CC% %CCFLAGS% -c %SRC%/kernel.c -o ./bin/kc.o
%CC% %CCFLAGS% -c %SRC%/modules/stdlib.c -o ./bin/stdlib.o
%CC% %CCFLAGS% -c %SRC%/modules/time.c -o ./bin/time.o
%CC% %CCFLAGS% -c %SRC%/modules/vga.c -o ./bin/vga.o
%CC% %CCFLAGS% -c %SRC%/modules/memory_manager.c -o ./bin/memory_manager.o
%CC% %CCFLAGS% -c %SRC%/modules/gdt.c -o bin/gdt.o
%CC% %CCFLAGS% -c %SRC%/modules/cmos.c -o bin/cmos.o
%CC% %CCFLAGS% -c %SRC%/modules/shell.c -o bin/shell.o
%CC% %CCFLAGS% -c %SRC%/modules/idt.c -o bin/idt.o
%CC% %CCFLAGS% -c %SRC%/modules/kbd.c -o bin/kbd.o
%CC% %CCFLAGS% -c %SRC%/modules/tty.c -o bin/tty.o
%CC% %CCFLAGS% -c %SRC%/modules/ports.c -o bin/ports.o
%CC% %CCFLAGS% -c %SRC%/modules/cpu_detect.c -o bin/cpu_detect.o
%CC% %CCFLAGS% -c %SRC%/modules/qemu_log.c -o bin/qemu_log.o
%CC% %LDFLAGS% -T %SRC%/link.ld -o bin/kernel.elf %OBJECTS%

echo Create iso
cp bin/kernel.elf isodir/boot/kernel.elf
cp %SRC%/grub.cfg isodir/boot/grub/grub.cfg
ubuntu run grub-mkrescue -o SynapseOS.iso isodir/


goto programm_done


:GetUnixTime
setlocal enableextensions
for /f %%x in ('wmic path win32_utctime get /format:list ^| findstr "="') do (
    set %%x)
set /a z=(14-100%Month%%%100)/12, y=10000%Year%%%10000-z
set /a ut=y*365+y/4-y/100+y/400+(153*(100%Month%%%100+12*z-3)+2)/5+Day-719469
set /a ut=ut*86400+100%Hour%%%100*3600+100%Minute%%%100*60+100%Second%%%100
endlocal & set "%1=%ut%" & goto :vars


:programm_done
echo Done
::Qemu config
::qemu-system-x86_64 -m 32 -kernel bin/kernel.elf -monitor stdio -serial file:Qemu_log.txt -no-reboot 
qemu-system-x86_64 -m 32 -cdrom SynapseOS.iso -monitor stdio -serial file:./run/Qemu_log.txt -no-reboot 

pause
exit
