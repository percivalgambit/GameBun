#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include "util/byte_size.h"

#include <array>
#include <cstdint>
#include <istream>
#include <string_view>
#include <vector>

namespace gamebun {

inline constexpr util::ByteSize kBankSize = 16 * util::kKilobytes;

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

struct CartridgeHardware {
  MemoryBankControllerType controller_type;
  bool has_ram;
  bool has_sram;
  bool has_battery;
  bool has_rumble;
  bool has_timer;
};

struct CartridgeHeader {
  bool color_gb;
  bool super_gb;
  bool japanese_game;
  std::string_view title;
  std::uint16_t license_code;
  CartridgeHardware hardware;
  util::ByteSize rom_size;
  util::ByteSize ram_size;
  uint8_t version;
  uint8_t header_checksum;
  uint16_t global_checksum;
};

class Cartridge {
 public:
  explicit Cartridge(std::istream *program);

 private:
  void SetHardware(uint8_t byte);

  CartridgeHeader header_;
  std::vector<std::array<uint8_t, kBankSize.value()>> rom_banks_;
};

}  // namespace gamebun

#endif  // CARTRIDGE_H_
