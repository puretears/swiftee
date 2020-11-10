// #include <vector>
#include "unicode.h"

using std::vector;

uint32_t unicode::get_code_point() const {
  return code_point;
}

uint32_t unicode::number_of_bytes() const {
  if (code_point <= 0x7F) {
    return 1; // ASCII
  }

  if (code_point <= 0x7FF) {
    return 2; // 110xxxxx 10xxxxxx
  }

  if (code_point <= 0xFFFF) {
    return 3; // 1110xxxx 10xxxxxx 10xxxxxx
  }

  if (code_point <= 0x10FFFF) {
    return 4; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
  }

  return 0; // Invalid code point
}

vector<uint8_t> unicode::encode_utf8() const {
  vector<uint8_t> buf {};

  if (code_point <= 0x7F) {
    buf.push_back(code_point & 0x7Fu);
  }
  else if (code_point <= 0x7FF) {
    uint8_t high = ((code_point & 0x7C0u) >> 6u) | 0xC0u;
    buf.push_back(high);

    uint8_t low = (code_point & 0x3Fu) | 0x80u;
    buf.push_back(low);
  }
  else if (code_point <= 0xFFFF) {
    uint8_t byte2 = ((code_point & 0xF000u) >> 12u) | 0xE0u;
    buf.push_back(byte2);

    uint8_t byte1 = ((code_point & 0xFC0u) >> 6u) | 0x80u;
    buf.push_back(byte1);

    uint8_t byte0 = (code_point & 0x3Fu) | 0x80u;
    buf.push_back(byte0);
  }
  else if (code_point <= 0x10FFFF) {
    uint8_t byte3 = ((code_point & 0x1C000u) >> 18u) | 0xF0u;
    buf.push_back(byte3);

    uint8_t byte2 = ((code_point & 0x3F000u) >> 12u) | 0x80u;
    buf.push_back(byte2);

    uint8_t byte1 = ((code_point & 0xFC0u) >> 6u) | 0x80u;
    buf.push_back(byte1);

    uint8_t byte0 = (code_point & 0x3Fu) | 0x80u;
    buf.push_back(byte0);
  }

  return buf;
}

uint32_t unicode::decode_utf8(const uint8_t *bytes_ptr, uint32_t length) {
  if (*bytes_ptr <= 0x7F) {
    code_point = *bytes_ptr;
    return code_point;
  }

  uint32_t value = 0, remains = 0;

  if ((*bytes_ptr & 0xE0u) == 0xC0u) {
    // 2 bytes unicode
    value = *bytes_ptr & 0x1Fu;
    remains = 1;
  }
  else if ((*bytes_ptr & 0xF0u) == 0xE0u) {
    // 3 bytes unicode
    value = *bytes_ptr & 0x0Fu;
    remains = 2;
  }
  else if ((*bytes_ptr & 0xF8u) == 0xF0u) {
    // 4 bytes unicode
    value = *bytes_ptr & 0x07u;
    remains = 3;
  }

  if (remains > length - 1) { return -1; }

  while (remains > 0) {
    ++bytes_ptr;
    --remains;

    if ((*bytes_ptr & 0xC0u) != 0x80u) { return -1; }

    value = value << 6u | (*bytes_ptr & 0x3Fu);
  }

  code_point = value;
  return value;
}

uint32_t unicode::number_of_bytes(uint8_t byte) {
  if ((byte & 0xC0u) == 0x80u) {
    return 0; // The middle byte of unicode.
  }

  if ((byte & 0xF8u) == 0xF0u) { return 4; }

  if ((byte & 0xF0u) == 0xE0u) { return 3; }

  if ((byte & 0xE0u) == 0xC0u) { return 2; }

  return 1;
}