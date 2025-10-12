
# Makefile for Solarix (32-bit, Multiboot, GRUB)
# Requires: i686-elf-gcc, i686-elf-ld, i686-elf-as, nasm, grub-mkrescue, qemu-system-i386

ISO = solarix.iso
KERNEL_BIN = kernel.bin
TARGET = i686-elf

CC = $(TARGET)-gcc
LD = $(TARGET)-ld
AS = nasm

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T kernel/kernel.ld -nostdlib

OBJDIR = build
OBJS = $(OBJDIR)/boot.o $(OBJDIR)/kernel.o

.PHONY: all clean run iso

all: $(ISO)

$(ISO): $(KERNEL_BIN)
	@mkdir -p isodir/boot/grub
	cp $< isodir/boot/
	cp boot/grub/grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir >/dev/null 2>&1

$(KERNEL_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/boot.o: kernel/boot.s
	@mkdir -p $(OBJDIR)
	$(AS) -f elf32 $< -o $@

$(OBJDIR)/kernel.o: kernel/kernel.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(ISO) isodir $(KERNEL_BIN)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)
