#pragma once
#include "auto_release.hpp"
namespace duskland::core {
template <class T> class singleton {
public:
  static core::auto_release<T> get() {
    static core::auto_release<T> instance = new T;
    return instance;
  }
};
} // namespace duskland::core