#pragma once
#include "../consts.h"

// terminal handling
void terminal_clear() {
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = ((uint16_t)terminal_color << 8) | ' ';
        }
    }
    terminal_row = 0;
    terminal_col = 0;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_row++;
        terminal_col = 0;
        if (terminal_row >= VGA_HEIGHT) terminal_row = 0;
        return;
    }

    size_t index = terminal_row * VGA_WIDTH + terminal_col;
    VGA_MEMORY[index] = ((uint16_t)terminal_color << 8) | c;

    terminal_col++;
    if (terminal_col == VGA_WIDTH) {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) terminal_row = 0;
    }
}

void terminal_write(const char *str) {
    while (*str) terminal_putchar(*str++);
}

void terminal_writeln(const char *str) {
    terminal_write(str);
    terminal_putchar('\n');
}

// leyboard input self explanatory twin idk why i added a comment
static char scancode_to_ascii(uint8_t scancode) {
    static const char keymap[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
        '9', '0', '-', '=', '\b', '\t',
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
        '[', ']', '\n', 0,
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
        '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        0, '*', 0, ' ', 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        '-', 0, 0, '+', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
    };

    return (scancode < 128) ? keymap[scancode] : 0;
}

static uint8_t read_keyboard_scan_code() {
    while ((inb(0x64) & 1) == 0);
    return inb(0x60);
}

void terminal_input(char *buffer) {
    size_t buffer_size = sizeof(buffer);
    size_t pos = 0;
    while (1) {
        uint8_t scancode = read_keyboard_scan_code();
        if (scancode & 0x80) continue;

        char c = scancode_to_ascii(scancode);
        if (c == 0) continue;

        if (c == '\n' || c == '\r') {
            terminal_putchar('\n');
            buffer[pos] = '\0';
            break;
        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                if (terminal_col > 0) terminal_col--;
                else if (terminal_row > 0) {
                    terminal_row--;
                    terminal_col = VGA_WIDTH - 1;
                }
                size_t index = terminal_row * VGA_WIDTH + terminal_col;
                VGA_MEMORY[index] = ((uint16_t)terminal_color << 8) | ' ';
            }
        } else if (pos < buffer_size - 1) {
            buffer[pos++] = c;
            terminal_putchar(c);
        }
    }
}
