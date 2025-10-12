#include <stdint.h>
// I/O ports
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    __asm__ volatile("outb %%al, $0x80" : : "a"(0));
}
