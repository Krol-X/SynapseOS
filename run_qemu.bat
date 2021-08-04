:: after -m written ram size in mb
qemu-system-i386 -m 32 -cdrom SynapseOS.iso -monitor stdio -serial file:Qemu_log.txt -no-reboot 
:: -d cpu_reset