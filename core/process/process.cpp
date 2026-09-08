#include "process.h"
#include "../memory/memory.h"

namespace solarix {
namespace process {

static process_control_block* process_table[MAX_PROCESSES];
static uint32_t next_pid = 1;
static process_control_block* current = NULL;

int initialize() {
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        process_table[i] = NULL;
    }
    next_pid = 1;
    return STATUS_OK;
}

process_control_block* create(uint32_t entry_point) {
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        if (!process_table[i]) {
            process_control_block* pcb = 
                (process_control_block*)memory::allocate(sizeof(process_control_block));
            
            if (!pcb) return NULL;
            
            void* stack = memory::allocate(KERNEL_STACK_SIZE);
            if (!stack) return NULL;
            
            pcb->process_id = next_pid++;
            pcb->state = STATE_READY;
            pcb->stack_pointer = (uint32_t)stack + KERNEL_STACK_SIZE - 4;
            pcb->memory_start = (uint32_t)stack;
            pcb->memory_size = KERNEL_STACK_SIZE;
            
            process_table[i] = pcb;
            
            if (!current) current = pcb;
            
            return pcb;
        }
    }
    return NULL;
}

int terminate(uint32_t pid) {
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i] && process_table[i]->process_id == pid) {
            process_table[i] = NULL;
            return STATUS_OK;
        }
    }
    return STATUS_ERROR;
}

process_control_block* get_current() {
    return current;
}

}
}
