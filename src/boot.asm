[BITS 16]
ORG 0x7C00
mov ah, 0x00
mov al, 0x03
int 0x10

cli

xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7C00

mov si, file
call print_string
mov si, msg
call print_string
load_kernel:
    mov bx, 0x1000        ; load kernel to 0x1000
    mov dl, 0x80          ; drive number

    mov ah, 0x02          ; our read sector
    mov al, 1             ; number of sectors
    mov ch, 0             ; cylinder?? idk what that is lol
    mov cl, 2             ; sector number
    mov dh, 0             ; head number
    int 0x13

    jc disk_error

    jmp 0x0000:0x1000      ; jump to kernel <3

disk_error:
    jmp $

print_string:
.loop:
    lodsb
    test al, al
    jz .done

    mov ah, 0x0E
    int 0x10
    jmp .loop

.done:
    ret

file db "BOOT.BIN", 0x0D, 0x0A, 0
msg db "solarix bootloader <3", 0x0D , 0x0A , 0
times 510-($-$$) db 0
dw 0xAA55