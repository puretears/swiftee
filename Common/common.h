#ifndef __INCLUDE_COMMON_H__
#define __INCLUDE_COMMON_H__

#include <cstdint>
#include <cstdlib>
#include <cstdio>

#define UNUSED __attribute__ ((unused))

#ifdef DEBUG
inline void ASSERT(bool condition, const char *msg) {
  if (!condition) {
    fprintf(stderr, "ASSERT failed! %s:%d in function %s(): %s\n",
      __FILE__, __LINE__, __func__, msg);
    abort();
  }
}
#endif
inline void ASSERT(bool condition, const char *msg) {}
#endif
