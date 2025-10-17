#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "utils/utils.h"
#include "consts.h"

#define MAX_INPUT_LENGTH 256
#define HTTP_RESPONSE_SIZE 2048

const char *user = "root";

static bool json_extract_string(const char *json, const char *key, char *out, size_t out_size) {
    char pattern[64];
    snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);

    const char *start = strstr(json, pattern);
    if (!start) return false;

    start += strlen(pattern);
    const char *end = strchr(start, '"');
    if (!end) return false;

    size_t len = (size_t)(end - start);
    if (len >= out_size) len = out_size - 1;

    memcpy(out, start, len);
    out[len] = '\0';

    return true;
}

static bool json_extract_number(const char *json, const char *key, char *out, size_t out_size) {
    char pattern[64];
    snprintf(pattern, sizeof(pattern), "\"%s\":", key);

    const char *start = strstr(json, pattern);
    if (!start) return false;

    start += strlen(pattern);

    const char *end1 = strchr(start, ',');
    const char *end2 = strchr(start, '}');
    const char *end = NULL;

    if (!end1) end = end2;
    else if (!end2) end = end1;
    else end = (end1 < end2) ? end1 : end2;

    if (!end) return false;

    size_t len = (size_t)(end - start);
    if (len >= out_size) len = out_size - 1;

    memcpy(out, start, len);
    out[len] = '\0';

    return true;
}

extern bool http_get(const char *url, char *response_buf, size_t bufsize);

void command_trace(const char *ip_str) {
    if (!ip_str || *ip_str == '\0') {
        terminal_writeln("Usage: trace -m <ip>");
        return;
    }

    char url[256];
    snprintf(url, sizeof(url), "http://ip-api.com/json/%s", ip_str);

    terminal_writeln("Fetching geolocation data...");

    char response[HTTP_RESPONSE_SIZE] = {0};
    if (!http_get(url, response, sizeof(response))) {
        terminal_writeln("Error: could not fetch geolocation data");
        return;
    }

    char buf[128];

    if (!json_extract_string(response, "status", buf, sizeof(buf))) {
        terminal_writeln("Error: invalid JSON response");
        return;
    }

    if (strcmp(buf, "fail") == 0) {
        if (json_extract_string(response, "message", buf, sizeof(buf))) {
            terminal_write("Trace failed: ");
            terminal_writeln(buf);
        } else {
            terminal_writeln("Trace failed: unknown error");
        }
        return;
    }

    terminal_writeln("Trace result:");

    if (json_extract_string(response, "query", buf, sizeof(buf))) {
        terminal_write("  IP: ");
        terminal_writeln(buf);
    }

    if (json_extract_string(response, "country", buf, sizeof(buf))) {
        terminal_write("  Country: ");
        terminal_writeln(buf);
    }

    if (json_extract_string(response, "regionName", buf, sizeof(buf))) {
        terminal_write("  Region: ");
        terminal_writeln(buf);
    }

    if (json_extract_string(response, "city", buf, sizeof(buf))) {
        terminal_write("  City: ");
        terminal_writeln(buf);
    }

    if (json_extract_number(response, "lat", buf, sizeof(buf))) {
        char latbuf[64];
        snprintf(latbuf, sizeof(latbuf), "  Latitude: %s", buf);
        terminal_writeln(latbuf);
    }

    if (json_extract_number(response, "lon", buf, sizeof(buf))) {
        char lonbuf[64];
        snprintf(lonbuf, sizeof(lonbuf), "  Longitude: %s", buf);
        terminal_writeln(lonbuf);
    }

    if (json_extract_string(response, "isp", buf, sizeof(buf))) {
        terminal_write("  ISP: ");
        terminal_writeln(buf);
    }
}

void handle_command(const char *cmd, const char *user) {
    if (!cmd) return;

    if (strncmp(cmd, "trace -m ", 9) == 0) {
        command_trace(cmd + 9);
    } else if (strcmp(cmd, "help") == 0) {
        terminal_writeln("Available commands:");
        terminal_writeln("  help        - Show this help message");
        terminal_writeln("  clear       - Clear the screen");
        terminal_writeln("  about       - About Solarix OS");
        terminal_writeln("  trace -m ip - Geolocate an IP address");
    } else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(cmd, "about") == 0) {
        terminal_writeln("Solarix OS v1.0.1 - Minimal x86 OS");
        terminal_writeln("Created by Brainrot02 and _.eliluvsyou._");
        terminal_writeln("Logged in as root");
    } else {
        terminal_writeln("Unknown command. Type 'help' for list.");
    }
}

void main_handler() {
    char input_buffer[MAX_INPUT_LENGTH];
    terminal_clear();

    while (1) {
        terminal_write(user);
        terminal_write("@solarix:~# ");
        terminal_input(input_buffer);
        handle_command(input_buffer, user);
    }
}

void kernel_main() {
    terminal_clear();

    main_handler();
}
