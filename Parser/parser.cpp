#include <map>
#include "parser.h"
#include "../Common/unicode.h"
#include "../Common/utility.h"

std::map<token_type, string> parser::token_dic = std::map<token_type, string> {
    {token_type::T_UNKNOWN,       "T_UNKNOWN"},

    // Data type
    {token_type::T_NUM,           "T_NUM"},
    {token_type::T_STRING,        "T_STRING"},
    {token_type::T_ID,            "T_ID"},
    {token_type::T_INTERPOLATION, "T_INTERPOLATION"},

    // Keyword
    {token_type::T_VAR,           "T_VAR"},
    {token_type::T_FUNC,          "T_FUNC"},
    {token_type::T_IF,            "T_IF"},
    {token_type::T_ELSE,          "T_ELSE"},
    {token_type::T_TRUE,          "T_TRUE"},
    {token_type::T_FALSE,         "T_FALSE"},
    {token_type::T_WHILE,         "T_WHILE"},
    {token_type::T_FOR,           "T_FOR"},
    {token_type::T_BREAK,         "T_BREAK"},
    {token_type::T_CONTINUE,      "T_CONTINUE"},
    {token_type::T_RETURN,        "T_RETURN"},
    {token_type::T_NULL,          "T_NULL"},
    {token_type::T_CLASS,         "T_CLASS"},
    {token_type::T_SELF,          "T_SELF"},
    {token_type::T_STATIC,        "T_STATIC"},
    {token_type::T_IS,            "T_IS"},
    {token_type::T_SUPER,         "T_SUPER"},
    {token_type::T_IMPORT,        "T_IMPORT"},

    // Separator
    {token_type::T_COMMA,         "T_COMMA"},
    {token_type::T_COLON,         "T_COLON"},
    {token_type::T_LEFT_PAREN,    "T_LEFT_PAREN"},
    {token_type::T_RIGHT_PAREN,   "T_RIGHT_PAREN"},
    {token_type::T_LEFT_BRACKET,  "T_LEFT_BRACKET"},
    {token_type::T_RIGHT_BRACKET, "T_RIGHT_BRACKET"},
    {token_type::T_LEFT_BRACE,    "T_LEFT_BRACE"},
    {token_type::T_RIGHT_BRACE,   "T_RIGHT_BRACE"},
    {token_type::T_DOT,           "T_DOT"},
    {token_type::T_DOT_DOT,       "T_DOT_DOT"},

    // Operators
    {token_type::T_ADD,           "T_ADD"},
    {token_type::T_SUB,           "T_SUB"},
    {token_type::T_MUL,           "T_MUL"},
    {token_type::T_DIV,           "T_DIV"},
    {token_type::T_MOD,           "T_MOD"},

    {token_type::T_ASSIGN,        "T_ASSIGN"},

    {token_type::T_BIT_AND,       "T_BIT_AND"},
    {token_type::T_BIT_OR,        "T_BIT_OR"},
    {token_type::T_BIT_NOT,       "T_BIT_NOT"},
    {token_type::T_BIT_SHIFT_LEFT,  "T_BIT_SHIFT_LEFT"},
    {token_type::T_BIT_SHIFT_RIGHT, "T_BIT_SHIFT_RIGHT"},

    {token_type::T_LOGIC_AND,     "T_LOGIC_AND"},
    {token_type::T_LOGIC_OR,      "T_LOGIC_OR"},
    {token_type::T_LOGIC_NOT,     "T_LOGIC_NOT"},

    {token_type::T_EQUAL,         "T_EQUAL"},
    {token_type::T_NOT_EQUAL,     "T_NOT_EQUAL"},
    {token_type::T_GREAT,         "T_GREAT"},
    {token_type::T_GREAT_EQUAL,   "T_GREAT_EQUAL"},
    {token_type::T_LESS,          "T_LESS"},
    {token_type::T_LESS_EQUAL,    "T_LESS_EQUAL"},

    {token_type::T_QUESTION,      "T_QUESTION"},

    {token_type::T_EOF,           "T_EOF"} // Parser only
};

struct keyword_info {
  const char *keyword;
  uint8_t length;
  token_type type;
};

keyword_info keywords[] {
    {"var",       3, token_type::T_VAR},
    {"func",      4, token_type::T_FUNC},
    {"if",        2, token_type::T_IF},
    {"else",      4, token_type::T_ELSE},
    {"true",      4, token_type::T_TRUE},
    {"false",     5, token_type::T_FALSE},
    {"while",     5, token_type::T_WHILE},
    {"for",       3, token_type::T_FOR},
    {"break",     5, token_type::T_BREAK},
    {"continue",  8, token_type::T_CONTINUE},
    {"return",    6, token_type::T_RETURN},
    {"null",      4, token_type::T_NULL},
    {"class",     5, token_type::T_CLASS},
    {"self",      4, token_type::T_SELF},
    {"static",    6, token_type::T_STATIC},
    {"is",        2, token_type::T_IS},
    {"super",     5, token_type::T_SUPER},
    {"import",    6, token_type::T_IMPORT},
    {nullptr,     0, token_type::T_UNKNOWN}
};

bool parser::match_token(token_type expected) {
  return false;
}
void parser::parse_string() {
  byte_buffer.clear();

  while (true) {
    goto_next_char();

    if (curr_char == '\0') {
      utility::lex_error(this, "unterminated string!");
    }

    if (curr_char == '"') {
      curr_token.type = token_type::T_STRING;
      break;
    }

    if (curr_char == '%') {
      // Another interpolation in string
      if (!match_next_char('(')) {
        utility::lex_error(this, "( is expected after '%'.");
      }

      if (interpolation_right_parens_expected > 0) {
        utility::compile_error(this, "Nested interpolation is not supported yet.");
      }

      interpolation_right_parens_expected = 1;
      curr_token.type = token_type::T_INTERPOLATION;
      break;
    }

    if (curr_char == '\\') {
      // Escape string
      goto_next_char();

      switch (curr_char) {
        case '0':
          byte_buffer.push_back('\0');
          break;
        case 'a':
          byte_buffer.push_back('\a');
          break;
        case 'b':
          byte_buffer.push_back('\b');
          break;
        case 'f':
          byte_buffer.push_back('\f');
          break;
        case 'n':
          byte_buffer.push_back('\n');
          break;
        case 'r':
          byte_buffer.push_back('\r');
          break;
        case 't':
          byte_buffer.push_back('\t');
          break;
        case 'u':
          parse_unicode_code_point(byte_buffer);
          break;
        case '"':
          byte_buffer.push_back('\"');
          break;
        case '\\':
          byte_buffer.push_back('\\');
          break;
        default:
          utility::lex_error(this, "Invalid escape character: \\%c", curr_char);
          break;
      }
    }
    else {
      byte_buffer.push_back(curr_char);
    } // End if (curr_char == '\\')
  } // End while (true)

  byte_buffer.clear();
}

// Parse a string like "\uxxxx"
void parser::parse_unicode_code_point(vector<uint8_t> &buf) {
  uint32_t index = 0;
  uint8_t digit = 0;
  uint32_t value = 0;

  while (index < 4) {
    goto_next_char();

    if (curr_char == '\0') {
      utility::lex_error(this, "unterminated unicode");
    }

    if (curr_char >= '0' && curr_char <= '9') {
      digit = curr_char - '0';
    }
    else if (curr_char >= 'a' && curr_char <= 'f') {
      digit = curr_char - 'a' + 10;
    }
    else if (curr_char >= 'A' && curr_char <= 'F') {
      digit = curr_char - 'A' + 10;
    }
    else {
      utility::lex_error(this, "invalid unicode");
    }

    value = value * 16 | digit;
    ++index;
  }

  auto u1 = unicode(value);
  uint32_t count = u1.number_of_bytes();

  ASSERT(count != 0, "UTF-8 encoded bytes should be between 1 and 4.");

  auto bytes = u1.encode_utf8();
  for (auto byte : bytes) {
    buf.push_back(byte);
  }
}

void parser::parse_id(token_type type) {
  while (isalnum(curr_char) || curr_char == '_') {
    goto_next_char();
  }

  auto length = (uint32_t)(next_char - curr_token.begin - 1);

  if (type != token_type::T_UNKNOWN) {
    curr_token.type = type;
  }
  else {
    curr_token.type = id_or_keyword(curr_token.begin, length);
  }

  curr_token.length = length;
}

token_type parser::id_or_keyword(const char *start, uint32_t length) {
  uint32_t index = 0;

  while (keywords[index].keyword != nullptr) {
    if (keywords[index].length == length && memcmp(keywords[index].keyword, start, length) == 0) {
      return keywords[index].type;
    }

    ++index;
  }

  return token_type::T_ID;
}

parser::parser(const char *file, const char *code): file_name(file), source_code(code) {
  curr_char = *source_code;
  next_char = source_code + 1;

  curr_token = token();
  prev_token = curr_token;

  interpolation_right_parens_expected = 0;
  parent = nullptr;

  int_buffer = vector<int>();
  char_buffer = vector<char>();
  byte_buffer = vector<uint8_t>();
}

void parser::get_next_token() {
  prev_token = curr_token;
  skip_blanks();

  curr_token.type = token_type::T_EOF;
  curr_token.begin = next_char - 1;
  curr_token.length = 0;

  while (curr_char != '\0') {
    switch (curr_char) {
      case ',':
        curr_token.type = token_type::T_COMMA;
        break;
      case ':':
        curr_token.type = token_type::T_COLON;
        break;
      case '(':
        if (interpolation_right_parens_expected > 0) {
          ++interpolation_right_parens_expected;
        }

        curr_token.type = token_type::T_LEFT_PAREN;
        break;
      case ')':
        if (interpolation_right_parens_expected > 0) {
          --interpolation_right_parens_expected;

          if (interpolation_right_parens_expected == 0) {
            parse_string();
            // The token should be the identifier inside the parenthesis.
            break;
          }
        }

        curr_token.type = token_type::T_RIGHT_PAREN;
        break;
      case '[':
        curr_token.type = token_type::T_LEFT_BRACKET;
        break;
      case ']':
        curr_token.type = token_type::T_RIGHT_BRACKET;
        break;
      case '{':
        curr_token.type = token_type::T_LEFT_BRACE;
        break;
      case '}':
        curr_token.type = token_type::T_RIGHT_BRACE;
        break;
      case '.':
        if (match_next_char('.')) {
          curr_token.type = token_type::T_DOT_DOT;
        }
        else {
          curr_token.type = token_type::T_DOT;
        }
        break;
      case '=':
        if (match_next_char('=')) {
          curr_token.type = token_type::T_EQUAL;
        }
        else {
          curr_token.type = token_type::T_ASSIGN;
        }
        break;
      case '+':
        curr_token.type = token_type::T_ADD;
        break;
      case '-':
        curr_token.type = token_type::T_SUB;
        break;
      case '*':
        curr_token.type = token_type::T_MUL;
        break;
      case '/':
        if (match_next_char('/') || match_next_char('*')) {
          skip_comment();
          // Reset the current token and get the next token.
          curr_token.begin = next_char - 1;
          continue;
        }
        else {
          curr_token.type = token_type::T_DIV;
        }
        break;
      case '%':
        curr_token.type = token_type::T_MOD;
        break;
      case '&':
        if (match_next_char('&')) {
          curr_token.type = token_type::T_LOGIC_AND;
        }
        else {
          curr_token.type = token_type::T_BIT_AND;
        }
        break;
      case '|':
        if (match_next_char('|')) {
          curr_token.type = token_type::T_LOGIC_OR;
        }
        else {
          curr_token.type = token_type::T_BIT_OR;
        }
        break;
      case '~':
        curr_token.type = token_type::T_BIT_NOT;
        break;
      case '?':
        curr_token.type = token_type::T_QUESTION;
        break;
      case '>':
        if (match_next_char('=')) {
          curr_token.type = token_type::T_GREAT_EQUAL;
        }
        else if (match_next_char('>')) {
          curr_token.type = token_type::T_BIT_SHIFT_RIGHT;
        }
        else {
          curr_token.type = token_type::T_GREAT;
        }
        break;
      case '<':
        if (match_next_char('=')) {
          curr_token.type = token_type::T_LESS_EQUAL;
        }
        else if (match_next_char('<')) {
          curr_token.type = token_type::T_BIT_SHIFT_LEFT;
        }
        else {
          curr_token.type = token_type::T_LESS;
        }
        break;
      case '!':
        if (match_next_char('=')) {
          curr_token.type = token_type::T_NOT_EQUAL;
        }
        else {
          curr_token.type = token_type::T_LOGIC_NOT;
        }
        break;
      case '"':
        parse_string();
        break;
      default:
        if (isalpha(curr_char) || curr_char == '_') {
          parse_id(token_type::T_UNKNOWN);
        }
        else {
          if (curr_char == '#' && match_next_char('!')) {
            skip_lines();
            curr_token.begin = next_char - 1;
            continue;
          }

          utility::lex_error(this, "Unsupported char: \'%c\', quit.", curr_char);
        }

        return;
    } // End switch

    curr_token.length = (uint32_t)(next_char - curr_token.begin);
    goto_next_char();
    return;
  }
}

char parser::look_ahead() const {
  return *next_char;
}

void parser::goto_next_char() {
  curr_char = *next_char;
  ++next_char;
}

bool parser::match_next_char(char expected) {
  if (look_ahead() == expected) {
    goto_next_char();
    return true;
  }

  return false;
}

void parser::skip_lines() {
  goto_next_char();

  while (curr_char != '\0') {
    if (curr_char == '\n') {
      ++curr_token.line_no;
      goto_next_char();
      break;
    }

    goto_next_char();
  }
}

// Called after parsing the first '/'
void parser::skip_comment() {
  char c = look_ahead();

  if (curr_char == '/') {
    // Single line comment
    skip_lines();
  }
  else {
    // Block comment
    while (c != '*' && c != '\0') {
      goto_next_char();

      if (curr_char == '\n') {
        ++curr_token.line_no;
      }

      c = look_ahead();
    }

    if (match_next_char('*')) {
      if (!match_next_char('/')) {
        utility::lex_error(this, "Expect '/' after '*'.");
      }

      goto_next_char();
    }
    else {
      utility::lex_error(this, "Expect '*/' by the end of string.");
    }
  }

  skip_blanks();
}

void parser::skip_blanks() {
  while(isspace(curr_char)) {
    if (curr_char == '\n') {
      ++curr_token.line_no;
    }

    goto_next_char();
  }
}
