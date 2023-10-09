#pragma once
#include <cstddef>
#include <cstdint>
namespace duskland::core {
class object {
private:
  uint32_t _ref;

public:
  uint32_t add_ref();
  uint32_t sub_ref();
  object();
  virtual ~object();
  void *operator new(size_t size);
  void operator delete(void *buf);
  static void memory_leak_check();
};
} // namespace duskland::core
