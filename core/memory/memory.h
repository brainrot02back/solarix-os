#ifndef MEMORY_H
#define MEMORY_H

#include "../../types.h"

namespace solarix {
namespace memory {

void initialize(uint32_t total_bytes);
void* allocate(size_t size);
int free(void* ptr);
void clear(void* ptr, size_t size);
void copy(void* dst, const void* src, size_t size);

}
}

#endif
