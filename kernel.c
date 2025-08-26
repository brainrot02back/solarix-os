#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

uint16_t *const VGA_MEMORY = (uint16_t *)VGA_ADDRESS;
uint8_t terminal_color = 0x0F;

int terminal_row = 0;
int terminal_col = 0;

const char *user = "root";
bool authenticated = false;

#define MAX_INPUT_LENGTH 128

// I/O ports
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    __asm__ volatile("outb %%al, $0x80" : : "a"(0));
}

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

void terminal_input(char *buffer, size_t buffer_size) {
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

// Login system (auto-root)
void login_screen() {
    terminal_clear();
    terminal_writeln("Solarix OS Login");
    terminal_writeln("-----------------");

    terminal_write("Username: ");
    terminal_writeln(user); // Always root

    terminal_write("Password: ");
    terminal_writeln("********");

    terminal_writeln("\nLogin successful!");
    authenticated = true;
}

// command processing yes i know this was very much needed thank yall twins for being patient
void handle_command(const char *cmd, const char *user) {
    if (!cmd) return;

    if (strcmp(cmd, "help") == 0) {
        terminal_writeln("Available commands:");
        terminal_writeln("  help   - Show this help message");
        terminal_writeln("  clear  - Clear the screen");
        terminal_writeln("  about  - About Solarix OS");
    } else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(cmd, "about") == 0) {
        terminal_writeln("Solarix OS v1.0.1 - Minimal x86 OS");
        terminal_writeln("Created by Brainrot02 and _.shxdow._");
        terminal_write("Logged in as ");
        terminal_writeln(user ? user : "root");
    } else {
        terminal_writeln("Unknown command. Type 'help' for list.");
    }
}

// command loop for you skids to understand
void main_handler() {
    char input_buffer[MAX_INPUT_LENGTH];
    terminal_clear();

    while (1) {
        terminal_write("root@solarix:~# ");
        terminal_input(input_buffer, sizeof(input_buffer));
        handle_command(input_buffer, user);
    }
}

// kernel entry point :broken_heart: twin
void kernel_main() {
    terminal_clear();

    if (!authenticated) {
        login_screen();
    }

    if (authenticated) {
        main_handler();
    }
}
