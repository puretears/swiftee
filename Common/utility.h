#ifndef __INCLUDE_UTILITY_H__
#define __INCLUDE_UTILITY_H__

#include <sys/stat.h>

#include "common.h"
#include "../Parser/parser.h"

constexpr int DEFAULT_BUFFER_SIZE = 512;

enum class error_type {
  io, memory, runtime, lex, compile
};

struct utility {
  static uint32_t ceil_to_power2(uint32_t v) {
    v = v == 0 ? 1 : v;
    --v;

    v |= v >> 1u;
    v |= v >> 2u;
    v |= v >> 4u;
    v |= v >> 8u;
    v |= v >> 16u;

    ++v;
    return v;
  }

  static char *read_file(const char *path) {
    FILE *file = fopen(path, "r");

    if (file == nullptr) {
      io_error("Cannot open file \"%s\".\n", path);
    }

    struct stat file_stat;
    stat(path, &file_stat);
    size_t file_size = file_stat.st_size;

    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == nullptr) {
      memory_error("Cannot allocate memory for reading file \"%s\".\n", path);
    }

    size_t num_read = fread(file_content, sizeof(char), file_size, file);

    if (num_read < file_size) {
      io_error("Reading file \"%s\" failed.\n");
    }

    file_content[file_size] = '\0';
    fclose(file);

    return file_content;

  }

  static void io_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    report_error(nullptr, error_type::io, fmt, ap);
    va_end(ap);
  }

  static void memory_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    report_error(nullptr, error_type::memory, fmt, ap);
    va_end(ap);
  }

  static void lex_error(parser *pp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    report_error(pp, error_type::lex, fmt, ap);
    va_end(ap);
  }

  static void compile_error(parser *pp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    report_error(pp, error_type::compile, fmt, ap);
    va_end(ap);
  }

  static void runtime_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    report_error(nullptr, error_type::runtime, fmt, ap);
    va_end(ap);
  }

private:
  static void report_error(parser *pp, error_type et, const char *fmt, va_list ap) {
    char buffer[DEFAULT_BUFFER_SIZE] {'\0'};
    vsnprintf(buffer, DEFAULT_BUFFER_SIZE, fmt, ap);

    switch (et) {
      case error_type::io:
      case error_type::memory:
        fprintf(stderr, "%s:%d in function %s():%s\n", __FILE__, __LINE__, __func__, buffer);
        break;
      case error_type::lex:
      case error_type::compile:
        ASSERT(pp != nullptr, "Parser is null!!!");
        fprintf(stderr, "%s:%d \"%s\"\n", pp->file_name, pp->prev_token.line_no, buffer);
        break;
      case error_type::runtime:
        fprintf(stderr, "%s\n", buffer);
        break;
    }

    exit(1);
  }
};

#endif
