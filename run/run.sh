# after -m written ram size in mb
qemu-system-i386 -m 512 -cdrom SynapseOS.iso -monitor stdio -serial file:SynapseOS_log.txt -no-reboot # -d cpu_reset