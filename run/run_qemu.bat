::Qemu config
::qemu-system-x86_64 -m 32 -kernel bin/kernel.elf -monitor stdio -serial file:Qemu_log.txt -no-reboot 
qemu-system-x86_64 -m 32 -cdrom ../SynapseOS.iso -monitor stdio -serial file:Qemu_log.txt -no-reboot 

pause