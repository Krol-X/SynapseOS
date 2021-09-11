:: Qemu config

::qemu-system-x86_64 -m 32 -kernel bin/kernel.elf -monitor stdio -serial file:Qemu_log.txt -no-reboot 
qemu-system-i386 -m 32 -boot d -cdrom SynapseOS.iso -fda file:./run/A.img -fdb file:./run/B.img -monitor stdio -serial file:./run/Qemu_log.txt -no-reboot 

pause