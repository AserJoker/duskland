#pragma once
#include <cstddef>
namespace duskland::util {
int wcwidth(wchar_t ucs);
int wcswidth(const wchar_t *s, size_t n);
} // namespace duskland::util