#include "renderer.h"

namespace solarix {
namespace render {

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 200
#define GRAPHICS_MEMORY 0xA0000

static uint8_t* video_buffer = (uint8_t*)GRAPHICS_MEMORY;

void initialize() {
    // set graphics mode 13h (320x200, 256 colors)
    asm volatile(
        "mov $0x13, %%al\n"
        "mov $0x00, %%ah\n"
        "int $0x10\n"
        : : : "ax"
    );
}

void clear(uint8_t color) {
    for (int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        video_buffer[i] = color;
    }
}

void put_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= VIDEO_WIDTH || y < 0 || y >= VIDEO_HEIGHT) return;
    video_buffer[y * VIDEO_WIDTH + x] = color;
}

void draw_line(int x1, int y1, int x2, int y2, uint8_t color) {
    int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    int dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    int sx = (x2 > x1) ? 1 : -1;
    int sy = (y2 > y1) ? 1 : -1;
    int err = dx - dy;
    
    while (1) {
        put_pixel(x1, y1, color);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_circle(int cx, int cy, int radius, uint8_t color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (x <= y) {
        put_pixel(cx + x, cy + y, color);
        put_pixel(cx - x, cy + y, color);
        put_pixel(cx + x, cy - y, color);
        put_pixel(cx - x, cy - y, color);
        put_pixel(cx + y, cy + x, color);
        put_pixel(cx - y, cy + x, color);
        put_pixel(cx + y, cy - x, color);
        put_pixel(cx - y, cy - x, color);
        
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void draw_rect(int x, int y, int w, int h, uint8_t color) {
    for (int i = 0; i < w; i++) {
        put_pixel(x + i, y, color);
        put_pixel(x + i, y + h - 1, color);
    }
    for (int i = 0; i < h; i++) {
        put_pixel(x, y + i, color);
        put_pixel(x + w - 1, y + i, color);
    }
}

void fill_rect(int x, int y, int w, int h, uint8_t color) {
    for (int py = y; py < y + h; py++) {
        for (int px = x; px < x + w; px++) {
            put_pixel(px, py, color);
        }
    }
}

}
}
