//
// Created by Mars on 2020/11/8.
//

#ifndef TIDUS_TOKEN_H
#define TIDUS_TOKEN_H

#include <map>
#include <string>
#include "../Common/common.h"

using std::map;
using std::string;

enum class token_type {
  T_UNKNOWN,

  // Data type
  T_NUM,
  T_STRING,
  T_ID,
  T_INTERPOLATION,

  // Keyword
  T_VAR,
  T_FUNC,
  T_IF,
  T_ELSE,
  T_TRUE,
  T_FALSE,
  T_WHILE,
  T_FOR,
  T_BREAK,
  T_CONTINUE,
  T_RETURN,
  T_NULL,
  T_CLASS,
  T_SELF,
  T_STATIC,
  T_IS,
  T_SUPER,
  T_IMPORT,

  // Separator
  T_COMMA,
  T_COLON,
  T_LEFT_PAREN,
  T_RIGHT_PAREN,
  T_LEFT_BRACKET,
  T_RIGHT_BRACKET,
  T_LEFT_BRACE,
  T_RIGHT_BRACE,
  T_DOT,
  T_DOT_DOT,

  // Operators
  T_ADD,
  T_SUB,
  T_MUL,
  T_DIV,
  T_MOD,

  T_ASSIGN,

  T_BIT_AND,
  T_BIT_OR,
  T_BIT_NOT,
  T_BIT_SHIFT_LEFT,
  T_BIT_SHIFT_RIGHT,

  T_LOGIC_AND,
  T_LOGIC_OR,
  T_LOGIC_NOT,

  T_EQUAL,
  T_NOT_EQUAL,
  T_GREAT,
  T_GREAT_EQUAL,
  T_LESS,
  T_LESS_EQUAL,

  T_QUESTION,

  T_EOF // Parser only
};

struct token {
  token_type type;
  const char *begin;
  uint32_t length;
  uint32_t line_no;

  token() :type(token_type::T_UNKNOWN), begin(nullptr), length(0), line_no(1) {}
};

#endif //TIDUS_TOKEN_H
