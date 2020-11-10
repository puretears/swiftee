#ifndef __INCLUDE_UNICODE_H__
#define __INCLUDE_UNICODE_H__

#include <vector>
#include "common.h"

class unicode {
  uint32_t code_point;
public:
  explicit unicode(int val) : code_point(val) {}

  [[nodiscard]] uint32_t get_code_point() const;

  // Return the number of bytes in UTF8.
  [[nodiscard]] uint32_t number_of_bytes() const;

  // Encode code_point into UTF8 form in big-endian.
  [[nodiscard]] std::vector<uint8_t> encode_utf8() const;

  // Decode a series of big-endian bytes to a code_point.
  uint32_t decode_utf8(const uint8_t *bytes_ptr, uint32_t length);

  static uint32_t number_of_bytes(uint8_t byte);
};

#endif
