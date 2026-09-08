.PHONY: all clean rebuild check-deps install-deps iso run flash-usb

DISTRO_FILE := /etc/os-release
BUILD_DIR := build
ISO_DIR := $(BUILD_DIR)/isodir

CXXFLAGS := -fno-exceptions -fno-rtti -m32 -march=i386 -nostdlib -I.

# Object files to build
OBJS := $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/logger.o \
        $(BUILD_DIR)/memory.o $(BUILD_DIR)/process.o $(BUILD_DIR)/renderer.o

check-deps:
	@echo "checking dependencies..."
	@! command -v nasm &>/dev/null && echo "missing: nasm" || true
	@! command -v g++ &>/dev/null && echo "missing: g++" || true
	@! command -v ld &>/dev/null && echo "missing: ld" || true
	@! command -v grub-mkrescue &>/dev/null && echo "missing: grub-mkrescue (xorriso)" || true

install-deps:
	@if grep -q "Arch\|Manjaro\|omarchy" $(DISTRO_FILE) 2>/dev/null; then \
		sudo pacman -S --noconfirm nasm gcc binutils xorriso grub mtools qemu 2>/dev/null || true; \
	fi

# Bootloader
$(BUILD_DIR)/boot.o: boot/boot.asm
	@mkdir -p $(BUILD_DIR)
	nasm -f elf $< -o $@

# Kernel
$(BUILD_DIR)/kernel.o: core/kernel/kernel.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@ $(CXXFLAGS)

# Subsystems
$(BUILD_DIR)/logger.o: core/logging/logger.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/memory.o: core/memory/memory.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/process.o: core/process/process.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@ $(CXXFLAGS)

$(BUILD_DIR)/renderer.o: core/rendering/renderer.cpp
	@mkdir -p $(BUILD_DIR)
	g++ -c $< -o $@ $(CXXFLAGS)

# Link kernel
$(BUILD_DIR)/kernel.bin: $(OBJS)
	ld -T linker.ld $(OBJS) -o $@ -m elf_i386

# Create bootable ISO
iso: $(BUILD_DIR)/kernel.bin
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(BUILD_DIR)/kernel.bin $(ISO_DIR)/boot/
	@echo 'menuentry "solarix os" {' > $(ISO_DIR)/boot/grub/grub.cfg
	@echo '    multiboot /boot/kernel.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'set timeout=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $(BUILD_DIR)/solarix.iso $(ISO_DIR) 2>/dev/null
	@echo "✓ iso created: $(BUILD_DIR)/solarix.iso"

# Flash ISO to USB device
flash-usb: iso
	@if [ -z "$(DEVICE)" ]; then \
		echo "usage: make flash-usb DEVICE=/dev/sdX"; \
		echo ""; \
		echo "available usb devices:"; \
		lsblk -d -o NAME,SIZE,TYPE | grep disk; \
		exit 1; \
	fi
	@if [ ! -b "$(DEVICE)" ]; then \
		echo "error: $(DEVICE) is not a valid block device"; \
		exit 1; \
	fi
	@echo "⚠️  WARNING: This will erase all data on $(DEVICE)"; \
	@read -p "type 'yes' to continue: " confirm; \
	if [ "$$confirm" = "yes" ]; then \
		sudo dd if=$(BUILD_DIR)/solarix.iso of=$(DEVICE) bs=4M status=progress conv=fsync; \
		sudo sync; \
		echo "✓ flashed to $(DEVICE)"; \
	else \
		echo "aborted"; \
	fi

run: $(BUILD_DIR)/kernel.bin
	qemu-system-i386 -kernel $< -m 256

all: install-deps $(BUILD_DIR)/kernel.bin

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all
