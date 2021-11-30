#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include "memory.h"
#include "memory_bank_controller.h"
#include "util/byte_size.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <string_view>
#include <vector>

namespace gamebun {

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
  uint16_t license_code;
  CartridgeHardware hardware;
  size_t rom_bank_num;
  size_t ram_bank_num;
  uint8_t version;
  uint8_t header_checksum;
  uint16_t global_checksum;
};

struct Cartridge {
  explicit Cartridge(std::istream* program);

  CartridgeHeader header;
  std::vector<std::array<uint8_t, kRomBankSize.value()>> rom_banks;
};

}  // namespace gamebun

#endif  // CARTRIDGE_H_
