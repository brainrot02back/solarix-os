#pragma once
#include <stdint.h>
#include <stdbool.h>

// these are our values that never change
// or rarely change
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

// VGA memory pointer (points to VGA text buffer)
static uint16_t *const VGA_MEMORY = (uint16_t *)VGA_ADDRESS;

// default terminal color 
static uint8_t terminal_color = 0x0F; // i dont *think this changes but i cba to check

// these values change quite a lot
static int terminal_row = 0;
static int terminal_col = 0;

// user and authentication state
static const char *user = "root"; // todo: add a real user system instead of this
static bool authenticated = false;


#define MAX_INPUT_LENGTH 128
