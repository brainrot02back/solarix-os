#!/bin/bash

# solarix os installer
# easy hardware deployment and testing

set -e

BUILD_DIR="build"
ISO_FILE="$BUILD_DIR/solarix.iso"

print_banner() {
    echo ""
    echo "╔════════════════════════════════════════════════════════╗"
    echo "║            SOLARIX OS INSTALLER v0.1                  ║"
    echo "╚════════════════════════════════════════════════════════╝"
    echo ""
}

print_menu() {
    echo "choose an action:"
    echo ""
    echo "  1. install dependencies"
    echo "  2. build kernel"
    echo "  3. create bootable iso"
    echo "  4. test in qemu"
    echo "  5. flash to usb device"
    echo "  6. full setup (dependencies + build + iso)"
    echo "  0. exit"
    echo ""
}

check_deps() {
    local missing=0
    echo "checking dependencies..."
    echo ""
    
    local deps=("nasm" "g++" "ld" "grub-mkrescue" "mformat" "qemu-system-i386")
    for dep in "${deps[@]}"; do
        if command -v "$dep" &>/dev/null; then
            echo "  ✓ $dep found"
        else
            echo "  ✗ $dep missing"
            missing=1
        fi
    done
    
    echo ""
    return $missing
}

install_deps() {
    echo "installing build dependencies..."
    echo ""
    
    if grep -q "Arch\|Manjaro\|omarchy" /etc/os-release 2>/dev/null; then
        echo "detected arch-based distro (arch/manjaro/omarchy)"
        echo "installing via pacman..."
        echo ""
        sudo pacman -S --noconfirm nasm gcc binutils xorriso grub mtools qemu
    elif grep -q "Ubuntu\|Debian" /etc/os-release 2>/dev/null; then
        echo "detected debian-based distro (ubuntu/debian)"
        echo "installing via apt..."
        echo ""
        sudo apt-get update
        sudo apt-get install -y nasm gcc binutils xorriso grub-pc-bin grub-efi-amd64-bin mtools qemu qemu-system-i386
    elif grep -q "Fedora\|RHEL\|CentOS" /etc/os-release 2>/dev/null; then
        echo "detected fedora-based distro"
        echo "installing via dnf..."
        echo ""
        sudo dnf install -y nasm gcc binutils xorriso grub2-tools mtools qemu qemu-system-x86
    else
        echo "unknown distro. please install manually:"
        echo "  - nasm (assembler)"
        echo "  - gcc/g++ (compiler)"
        echo "  - binutils (linker)"
        echo "  - xorriso (iso creation)"
        echo "  - grub (bootloader)"
        echo "  - mtools (grub-mkrescue dependency)"
        echo "  - qemu (emulator)"
        return 1
    fi
    
    echo ""
    echo "✓ dependencies installed"
    echo ""
}

build_kernel() {
    if [ ! -f "Makefile" ]; then
        echo "error: Makefile not found. run from project root."
        return 1
    fi
    
    echo "building kernel..."
    echo ""
    make clean
    make all
    
    echo ""
    if [ -f "$BUILD_DIR/kernel.bin" ]; then
        local size=$(du -h "$BUILD_DIR/kernel.bin" | cut -f1)
        echo "✓ kernel built successfully ($size)"
    else
        echo "✗ build failed"
        return 1
    fi
    echo ""
}

create_iso() {
    if [ ! -f "$BUILD_DIR/kernel.bin" ]; then
        echo "error: kernel.bin not found. build first."
        return 1
    fi
    
    echo "creating bootable iso..."
    echo ""
    make iso
    
    echo ""
    if [ -f "$ISO_FILE" ]; then
        local size=$(du -h "$ISO_FILE" | cut -f1)
        echo "✓ iso created: $ISO_FILE ($size)"
    else
        echo "✗ iso creation failed"
        return 1
    fi
    echo ""
}

test_qemu() {
    if [ ! -f "$BUILD_DIR/kernel.bin" ]; then
        echo "error: kernel.bin not found. build first."
        return 1
    fi
    
    echo "launching qemu (press ctrl+c to exit)..."
    echo ""
    make run
    echo ""
}

flash_usb() {
    if [ ! -f "$ISO_FILE" ]; then
        echo "error: iso not found. create iso first."
        return 1
    fi
    
    echo "usb flash tool"
    echo ""
    echo "available usb devices:"
    echo ""
    lsblk -d -o NAME,SIZE,TYPE | grep disk
    echo ""
    read -p "enter device name (e.g. sda, sdb): " device
    device="/dev/$device"
    
    if [ ! -b "$device" ]; then
        echo "error: $device is not a valid block device"
        return 1
    fi
    
    echo ""
    echo "⚠️  WARNING: This will erase all data on $device"
    echo "data cannot be recovered."
    echo ""
    read -p "type 'yes' to continue: " confirm
    
    if [ "$confirm" != "yes" ]; then
        echo "aborted"
        return 0
    fi
    
    echo ""
    echo "flashing iso to $device..."
    echo ""
    sudo dd if="$ISO_FILE" of="$device" bs=4M status=progress conv=fsync
    sudo sync
    echo ""
    echo "✓ flashed successfully"
    echo ""
    echo "you can now:"
    echo "  1. insert usb into target machine"
    echo "  2. boot from usb (usually F12, F2, or DEL during boot)"
    echo "  3. select solarix os from grub menu"
    echo ""
}

full_setup() {
    echo "performing full setup..."
    echo ""
    install_deps
    build_kernel
    create_iso
    
    echo "✓ full setup complete"
    echo ""
    echo "next steps:"
    echo "  - run 'make run' to test in qemu"
    echo "  - run './install.sh' and select option 5 to flash to usb"
    echo ""
}

main() {
    print_banner
    
    # check if in project root
    if [ ! -f "Makefile" ] || [ ! -d "core" ] || [ ! -d "boot" ]; then
        echo "error: not in project root directory"
        exit 1
    fi
    
    while true; do
        print_menu
        read -p "enter choice [0-6]: " choice
        echo ""
        
        case $choice in
            1)
                install_deps
                ;;
            2)
                build_kernel
                ;;
            3)
                create_iso
                ;;
            4)
                test_qemu
                ;;
            5)
                flash_usb
                ;;
            6)
                full_setup
                ;;
            0)
                echo "goodbye!"
                exit 0
                ;;
            *)
                echo "invalid choice"
                echo ""
                ;;
        esac
    done
}

main "$@"
