#pragma once
// these are our values that never change
// or rarely change
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000
#define MAX_INPUT_LENGTH 128
uint16_t *const VGA_MEMORY = (uint16_t *)VGA_ADDRESS;

uint8_t terminal_color = 0x0F; // i dont *think this changes but i cba to check


// these values change quite a lot
int terminal_row = 0;
int terminal_col = 0;

const char *user = "root"; // todo: add a real user system instead of this
bool authenticated = false;

