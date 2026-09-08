#include "../../types.h"
#include "../logging/logger.h"
#include "../memory/memory.h"
#include "../process/process.h"
#include "../rendering/renderer.h"

extern "C" void kernel_main() {
    solarix::logger::initialize();
    solarix::logger::log_raw("solarix os v0.1");
    
    solarix::memory::initialize(KERNEL_HEAP_SIZE);
    solarix::logger::log_ok("memory initialized");
    
    solarix::process::initialize();
    solarix::logger::log_ok("process system initialized");
    
    solarix::render::initialize();
    solarix::logger::log_ok("renderer initialized");
    
    solarix::render::clear(solarix::render::BLACK);
    
    solarix::render::draw_line(50, 50, 200, 50, solarix::render::GREEN);
    solarix::render::draw_line(50, 50, 50, 150, solarix::render::GREEN);
    solarix::render::draw_line(200, 50, 200, 150, solarix::render::GREEN);
    solarix::render::draw_line(50, 150, 200, 150, solarix::render::GREEN);
    
    solarix::render::draw_circle(160, 100, 30, solarix::render::RED);
    
    solarix::render::fill_rect(220, 80, 40, 40, solarix::render::YELLOW);
    
    solarix::logger::log_ok("kernel running");
    
    while (1) {
        asm volatile("hlt");
    }
}
