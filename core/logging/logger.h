#ifndef LOGGER_H
#define LOGGER_H

#include "../../types.h"

namespace solarix {
namespace logger {

void initialize();
void put_char(char c);
void put_string(const char* str);
void clear_screen();
void log_ok(const char* msg);
void log_fail(const char* msg);
void log_info(const char* msg);
void log_raw(const char* msg);

}
}

#endif
