#include "../include/floppy.h"


// standard base address of the primary floppy controller
static const int floppy_base = 0x03f0;

// standard IRQ number for floppy controllers
static const int floppy_irq = 6;

// The registers of interest. There are more, but we only use these here.
enum floppy_registers {
	FLOPPY_DOR  = 2,  // digital output register
	FLOPPY_MSR  = 4,  // master status register, read only
	FLOPPY_FIFO = 5,  // data FIFO, in DMA operation for commands
	FLOPPY_CCR  = 7	// configuration control register, write only
};

// The commands of interest. There are more, but we only use these here.
enum floppy_commands {
	CMD_SPECIFY = 3,				// SPECIFY
	CMD_WRITE_DATA = 5,			// WRITE DATA
	CMD_READ_DATA = 6,			// READ DATA
	CMD_RECALIBRATE = 7,		// RECALIBRATE
	CMD_SENSE_INTERRUPT = 8,	// SENSE INTERRUPT
	CMD_SEEK = 15,				// SEEK
};

static const char * drive_types[8] = {
	"none",
	"360kB 5.25",
	"1.2MB 5.25",
	"720kB 3.5",

	"1.44MB 3.5",
	"2.88MB 3.5",
	"unknown type",
	"unknown type"
};



// Obviously you'd have this return the data, start drivers or something.
void floppy_detect_drives() {

	outb(0x70, 0x10);
	unsigned drives = inb(0x71);
	tty_printf("\nFloppy:\n");
	tty_printf(" - Floppy drive A: %s\n", drive_types[drives >> 4]);
	tty_printf(" - Floppy drive B: %s\n\n", drive_types[drives & 0xf]);
	floppy_write_cmd((int)drives >> 4, 'A');
	floppy_write_cmd((int)drives & 0xf, 'B');
	tty_printf("Floppy drive A data: \n [%s] \n", floppy_read_data(0));
	tty_printf("Floppy drive B data: \n [%s] \n", floppy_read_data((int)drives & 0xf));
	
}

//
// The MSR byte: [read-only]
// -------------
//
//  7	6	5	4	3	2	1	0
// MRQ DIO NDMA BUSY ACTD ACTC ACTB ACTA
//
// MRQ is 1 when FIFO is ready (test before read/write)
// DIO tells if controller expects write (1) or read (0)
//
// NDMA tells if controller is in DMA mode (1 = no-DMA, 0 = DMA)
// BUSY tells if controller is executing a command (1=busy)
//
// ACTA, ACTB, ACTC, ACTD tell which drives position/calibrate (1=yes)
//
//

void floppy_write_cmd(int base, char cmd) {
	int i; // do timeout
	for(i = 0; i < 6000; i++) {
		io_wait(); // sleep
		if(0x80 & inb(base+FLOPPY_MSR)) {
			return (void) outb(base+FLOPPY_FIFO, cmd);
		}
	}
}

char *floppy_read_data(int base) {
	char str[2048];
	char *str2;
	qemu_printf("\nfloppy_read_data:\n");

	int i; // do timeout
	for(i = 0; i < 2048; i++) {
		qemu_printf("\ntry read: %d",i);

		if(0x80 & inb(base+FLOPPY_MSR)) {
			qemu_printf("\nRead: %c",inb(base+FLOPPY_FIFO));
			str[i] = inb(base+FLOPPY_FIFO);
		}
	}
	qemu_printf("\nReading end.\n");
	str2 = str;
	return str2;
}

