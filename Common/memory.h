#ifndef __INCLUDE_MEMORY_H__
#define __INCLUDE_MEMORY_H__

#include "common.h"
#include "../VM/vm.h"

class memory {
public:
  vm *pvm;

public:
  memory() {
    pvm = new vm();
  }

  ~memory() {
    delete pvm;
  }

  template<typename T> T *alloc() {
    /* nullptr has the same effect as calling std::malloc(new_size).*/
    return static_cast<T*>(manager(nullptr, 0, sizeof(T)));
  }

  template<typename T> T *alloc_extra(uint32_t extra_size) {
    return static_cast<T*>(manager(nullptr, 0, sizeof(T) + extra_size));
  }

  template<typename T> T *alloc_array(uint32_t count) {
    return static_cast<T*>(manager(nullptr, 0, sizeof(T) * count));
  }

  template<typename T> void dealloc(T *ptr) {
    manager(ptr, 0, 0);
  }

  template<typename T> void dealloc_array(T* arr, uint32_t count) {
    manager(arr, sizeof(arr[0]) * count, 0);
  }

private:
  void *manager(void *ptr, uint32_t old_size, uint32_t new_size) const {
    pvm->allocated_bytes += (new_size - old_size);

    if (new_size == 0) {
      free(ptr);
      return nullptr;
    }

    return realloc(ptr, new_size);
  }
};

#endif
