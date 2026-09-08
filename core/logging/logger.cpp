#include "logger.h"

namespace solarix {
namespace logger {

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;
static uint8_t* vga_buffer = (uint8_t*)VGA_MEMORY;

static uint8_t make_color(uint8_t fg, uint8_t bg) {
    return (bg << 4) | fg;
}

static uint16_t make_entry(char c, uint8_t color) {
    return ((uint16_t)c) | (((uint16_t)color) << 8);
}

static void update_cursor() {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    asm volatile(
        "mov $0x3D4, %%dx\n"
        "mov $0x0F, %%al\n"
        "out %%al, %%dx\n"
        "mov $0x3D5, %%dx\n"
        "mov %0, %%ax\n"
        "out %%ax, %%dx\n"
        : : "r"(pos) : "ax", "dx"
    );
}

void initialize() {
    cursor_x = 0;
    cursor_y = 0;
    clear_screen();
}

void put_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c != '\r') {
        uint8_t color = make_color(7, 0);
        uint16_t index = cursor_y * VGA_WIDTH + cursor_x;
        *(uint16_t*)(vga_buffer + index * 2) = make_entry(c, color);
        cursor_x++;
    }
    
    if (cursor_y >= VGA_HEIGHT) {
        cursor_y = VGA_HEIGHT - 1;
    }
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
}

void put_string(const char* str) {
    if (!str) return;
    while (*str) {
        put_char(*str++);
    }
    update_cursor();
}

void clear_screen() {
    uint8_t color = make_color(7, 0);
    for (uint32_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        *(uint16_t*)(vga_buffer + i * 2) = make_entry(' ', color);
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

void log_ok(const char* msg) {
    put_string("[ok] ");
    put_string(msg);
    put_char('\n');
}

void log_fail(const char* msg) {
    put_string("[fail] ");
    put_string(msg);
    put_char('\n');
}

void log_info(const char* msg) {
    put_string("[info] ");
    put_string(msg);
    put_char('\n');
}

void log_raw(const char* msg) {
    put_string(msg);
    put_char('\n');
}

}
}
