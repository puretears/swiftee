#ifndef __INCLUDE_VM_H__
#define __INCLUDE_VM_H__

#include "../Common/common.h"
#include "../Parser/parser.h"

struct vm {
  uint32_t allocated_bytes;
  parser *curr_parser;

  vm();
};

#endif
