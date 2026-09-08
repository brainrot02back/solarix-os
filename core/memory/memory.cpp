#include "memory.h"

namespace solarix {
namespace memory {

static uint32_t heap_start = 0x100000;
static uint32_t heap_current = 0x100000;
static uint32_t heap_end = 0x100000 + KERNEL_HEAP_SIZE;

void initialize(uint32_t total_bytes) {
    heap_current = heap_start;
    heap_end = heap_start + total_bytes;
}

void* allocate(size_t size) {
    if (size == 0) return NULL;
    
    size = (size + 15) & ~15;
    
    if (heap_current + size > heap_end) {
        return NULL;
    }
    
    void* ptr = (void*)heap_current;
    heap_current += size;
    return ptr;
}

int free(void* ptr) {
    return STATUS_OK;
}

void clear(void* ptr, size_t size) {
    if (!ptr) return;
    uint8_t* p = (uint8_t*)ptr;
    for (size_t i = 0; i < size; i++) {
        p[i] = 0;
    }
}

void copy(void* dst, const void* src, size_t size) {
    if (!dst || !src) return;
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;
    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
}

}
}
