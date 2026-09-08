#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;
typedef unsigned int size_t;
typedef int ssize_t;

#define NULL 0
#define STATUS_OK 0
#define STATUS_ERROR -1

#define KERNEL_HEAP_SIZE 0x100000
#define KERNEL_STACK_SIZE 0x4000
#define MAX_PROCESSES 256

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

enum process_state_t {
    STATE_NEW = 0,
    STATE_READY = 1,
    STATE_RUNNING = 2,
    STATE_WAITING = 3,
    STATE_TERMINATED = 4
};

struct process_control_block {
    uint32_t process_id;
    process_state_t state;
    uint32_t stack_pointer;
    uint32_t memory_start;
    size_t memory_size;
};

#endif
