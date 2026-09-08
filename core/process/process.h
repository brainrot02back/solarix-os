#ifndef PROCESS_H
#define PROCESS_H

#include "../../types.h"

namespace solarix {
namespace process {

int initialize();
process_control_block* create(uint32_t entry_point);
int terminate(uint32_t pid);
process_control_block* get_current();

}
}

#endif
