#include "Common/common.h"
#include "Common/memory.h"
#include "Common/utility.h"
#include "Common/unicode.h"
#include "Parser/parser.h"
#include "Parser/token.h"

#include <cstdio>
#include <iostream>
using namespace std;

void test1() {
  memory mm;

  auto p_num = mm.alloc<int>();
  cout<<"vm: allocated "<<mm.pvm->allocated_bytes<<" bytes."<<endl;

  auto p_arr = mm.alloc_array<int>(4);
  cout<<"vm: allocated "<<mm.pvm->allocated_bytes<<" bytes."<<endl;

  mm.dealloc_array(p_arr, 4);
  cout<<"vm: allocated "<<mm.pvm->allocated_bytes<<" bytes."<<endl;

  cout<<utility::read_file("../main.cpp")<<endl;

  unicode f = unicode { 0x548C };

  for(auto b : f.encode_utf8()) {
    printf("%x ", b);
  }

  uint8_t val[3] { 0xE2, 0x9D, 0x83 };
  f.decode_utf8(val, 3);

  printf("%x", f.get_code_point());
}

int main() {
  const char *source_code = utility::read_file("/Users/puretears/Projects/Tidus/token.list");
  parser p("token.list", source_code);

  while (p.curr_token.type != token_type::T_EOF) {
    p.get_next_token();

    printf("%dL-: %s\t[", p.curr_token.line_no, parser::token_dic[p.curr_token.type].c_str());

    uint32_t index = 0;
    while (index < p.curr_token.length) {
      printf("%c", *(p.curr_token.begin + index));
      ++index;
    }

    printf("]\n");
  }

  return 0;
}