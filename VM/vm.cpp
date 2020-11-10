#include <cstdlib>
#include "vm.h"

vm::vm() {
  this->allocated_bytes = 0;
  this->curr_parser = nullptr;
}
