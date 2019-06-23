#ifndef MEMORY_H_
#define MEMORY_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "address.h"
#include "memory_bank_controller.h"
#include "util/byte_size.h"

namespace gamebun {

inline constexpr util::ByteSize kRomBankSize = 16 * util::kKilobytes;
inline constexpr util::ByteSize kRamBankSize = 8 * util::kKilobytes;

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
