#ifndef MEMORY_H_
#define MEMORY_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "util/byte_size.h"
#include "util/strong_int.h"

namespace gamebun {

inline constexpr util::ByteSize kRomBankSize = 16 * util::kKilobytes;
inline constexpr util::ByteSize kRamBankSize = 8 * util::kKilobytes;

DEFINE_STRONG_INT_TYPE(Address, uint16_t)

class MemoryBankController;

// TODO: Add MMM01
// TODO: Add Pocket Camera
// TODO: Add Bandai TAMA5
// TODO: Add Hudson HuC-3
// TODO: Add Hudson HuC-1
enum class MemoryBankControllerType {
  kNone,
  kController1,
  kController2,
  kController3,
  kController5,
};

class Memory {
 public:
  Memory(
      const std::vector<std::array<uint8_t, kRomBankSize.value()>>& rom_banks,
      size_t ram_bank_num, MemoryBankControllerType controller_type);

  uint8_t Read(Address address) const;
  void Write(Address address, uint8_t value);

 private:
  const std::vector<std::array<uint8_t, kRomBankSize.value()>> rom_banks_;
  std::vector<std::array<uint8_t, kRamBankSize.value()>> ram_banks_;

  std::unique_ptr<MemoryBankController> memory_bank_controller_;
};

}  // namespace gamebun

#endif  // MEMORY_H_
