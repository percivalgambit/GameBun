#ifndef UTIL_STORAGE_UNIT_H_
#define UTIL_STORAGE_UNIT_H_

#include "util/strong_int.h"

#include <cstddef>
#include <cstdint>

namespace util {

DEFINE_STRONG_INT_TYPE(ByteSize, uint64_t)

inline constexpr ByteSize kBytes = ByteSize(1);
inline constexpr ByteSize kKilobytes = kBytes * 1024;
inline constexpr ByteSize kMegabytes = kKilobytes * 1024;
inline constexpr ByteSize kGigabytes = kMegabytes * 1024;
inline constexpr ByteSize kTerabytes = kGigabytes * 1024;

}  // namespace util

#endif  // UTIL_STORAGE_UNIT_H_
