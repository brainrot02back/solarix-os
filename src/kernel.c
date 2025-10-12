#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "utils/utils.h"
#include "consts.h"

// command processing
//  yes i know this was very much needed thank yall twins for being patient
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
        terminal_writeln("Created by Brainrot02 and _.eliluvsyou._");
        terminal_writeln("Logged in as root ");

    } else {
        terminal_writeln("Unknown command. Type 'help' for list.");
    }
}

void main_handler() {
    char input_buffer[MAX_INPUT_LENGTH];
    terminal_clear();

    while (1) {
        terminal_write("root@solarix:~# ");
        terminal_input(input_buffer);
        handle_command(input_buffer, user);
    }
}

// kernel entry point :broken_heart: twin
void kernel_main() {
    terminal_clear();

    // if (!authenticated) {
    //     login_screen();
    // }

    main_handler();
}
