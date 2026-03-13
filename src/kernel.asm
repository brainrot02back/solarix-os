[BITS 16]   
ORG 0x1000 ; where our kernel starts

mov si, file      
call print_string

mov si, msg2       
call print_string
   
mov si, msg3   
call print_string

jmp $

clear:
    mov ah, 0x00
    mov al, 0x03
    int 0x10

print_string:
.print_loop:
    lodsb               
    cmp al, 0           
    je .done

    mov ah, 0x0E      
    int 0x10
    jmp .print_loop

.done: 
    ret

file db "KERNEL.BIN", 0x0D, 0x0A, 0
msg2 db "welcome to solarix os", 0x0D, 0x0A, 0
msg3 db "conspiracy pegs me every night <3", 0x0D, 0x0A, 0