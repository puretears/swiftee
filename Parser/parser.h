#ifndef __INCLUDE_PARSER_H__
#define __INCLUDE_PARSER_H__

#include <map>
#include <vector>
#include "token.h"

using std::vector;

struct parser {
  static std::map<token_type, string> token_dic;

  const char *file_name;
  const char *source_code;

  const char *next_char;
  char curr_char;

  token curr_token;
  token prev_token;

  int interpolation_right_parens_expected;
  parser *parent;

  vector<int> int_buffer;
  vector<char> char_buffer;
  vector<uint8_t> byte_buffer;

  parser(const char *file, const char *code);

  void get_next_token();
  void parse_string();
  void parse_unicode_code_point(vector<uint8_t> &buf);
  void parse_id(token_type type);
  static token_type id_or_keyword(const char *start, uint32_t length);

  char look_ahead() const;
  void goto_next_char();
  bool match_next_char(char expected);

  void skip_lines();
  void skip_comment();
  void skip_blanks();

  bool match_token(token_type expected);

};

#endif
