.PHONY: all clean install uninstall

TARGET_DIR=../../bin/
SOURCE_DIR=./
INCLUDE_DIR=../include/

SOURCES=$(wildcard *.c)
OBJECTS=$(TARGET_DIR)$(SOURCES:.c=.o)

AS = "i686-elf-as"
CC = "i686-elf-gcc"
fasm = "FASM"

LDFLAGS=-O3 -ffreestanding -nostdlib

ifdef DEBUG
	CCFLAGS=-O3 -std=gnu99 -g -ffreestanding -Wall -Wextra
else
	CCFLAGS=-O3 -std=gnu99 -ffreestanding -Wall -Wextra
endif

all: $(OBJECTS) $(SOURCES) 

$(OBJECTS): $(SOURCES)
	echo $(CC) $@ -o $@.o
	$(CC) $(CFLAGS) $< -o $@


.c.o:
	echo $(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) $< -o $@