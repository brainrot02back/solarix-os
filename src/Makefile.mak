CROSS_PREFIX = i686-elf-
CC = $(CROSS_PREFIX)gcc
AS = $(CROSS_PREFIX)as
LD = $(CROSS_PREFIX)ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T link.ld -nostdlib

SRC = src/kernel.c src/boot.s
OBJ = build/kernel.o build/boot.o
ISO_DIR = isodir

all: build/kernel.bin

build/boot.o: src/boot.s
\t$(AS) $< -o $@

build/kernel.o: src/kernel.c
\t$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: build/boot.o build/kernel.o
\t$(LD) $(LDFLAGS) -o $@ $^

iso: build/kernel.bin
\tmkdir -p $(ISO_DIR)/boot/grub
\tcp build/kernel.bin $(ISO_DIR)/boot/
\tcp boot/grub.cfg $(ISO_DIR)/boot/grub/
\tgrub-mkrescue -o solarix.iso $(ISO_DIR)

run: iso
\tqemu-system-i386 -cdrom solarix.iso

clean:
\trm -rf build/* $(ISO_DIR) solarix.iso
