#include "cartridge.h"

#include "util/byte_size.h"
#include "util/logging.h"

#include <array>
#include <cstdint>
#include <istream>

namespace gamebun {

Cartridge::Cartridge(std::istream *program) {
  std::array<uint8_t, kBankSize.value()> header_bank;
  program->read(reinterpret_cast<char *>(header_bank.data()),
                kBankSize.value());

  header_.title =
      std::string_view(reinterpret_cast<char *>(&header_bank[0x134]), 16);
  header_.color_gb = header_bank[0x143] == 0x80;
  header_.super_gb = header_bank[0x146] == 0x03;
  SetHardware(header_bank[0x147]);

  const uint8_t rom_size_spec = header_bank[0x148];
  switch (rom_size_spec) {
    case 0x00:
      header_.rom_size = 32 * util::kKilobytes;
      break;
    case 0x01:
      header_.rom_size = 64 * util::kKilobytes;
      break;
    case 0x02:
      header_.rom_size = 128 * util::kKilobytes;
      break;
    case 0x03:
      header_.rom_size = 256 * util::kKilobytes;
      break;
    case 0x04:
      header_.rom_size = 512 * util::kKilobytes;
      break;
    case 0x05:
      header_.rom_size = 1 * util::kMegabytes;
      break;
    case 0x06:
      header_.rom_size = 2 * util::kMegabytes;
      break;
    case 0x52:
      header_.rom_size = 1152 * util::kKilobytes;
      break;
    case 0x53:
      header_.rom_size = 1280 * util::kKilobytes;
      break;
    case 0x54:
      header_.rom_size = 1536 * util::kKilobytes;
      break;
    default:
      FATAL("Invalid ROM size specifier: %x", rom_size_spec);
  }

  const uint8_t ram_size_spec = header_bank[0x149];
  switch (ram_size_spec) {
    case 0x00:
      break;
    case 0x01:
      header_.ram_size = 2 * util::kKilobytes;
      break;
    case 0x02:
      header_.ram_size = 8 * util::kKilobytes;
      break;
    case 0x03:
      header_.ram_size = 32 * util::kKilobytes;
      break;
    case 0x04:
      header_.ram_size = 128 * util::kKilobytes;
      break;
    default:
      FATAL("Invalid RAM size specifier: %x", ram_size_spec);
  }

  header_.japanese_game = header_bank[0x14A] == 0x00;
  const uint8_t old_license_code = header_bank[0x14B];
  const uint16_t new_license_code =
      static_cast<uint16_t>((header_bank[0x144] << 8) | header_bank[0x145]);
  header_.license_code =
      old_license_code == 0x33 ? new_license_code : old_license_code;
  header_.super_gb &= old_license_code != 0x33;
  header_.version = header_bank[0x14C];
  header_.header_checksum = header_bank[0x14D];
  header_.global_checksum =
      static_cast<uint16_t>((header_bank[0x14E] << 8) | header_bank[0x14F]);

  rom_banks_.push_back(header_bank);
  for (unsigned i = 1; i < (header_.rom_size / kBankSize).value(); i++) {
    if (program->eof()) {
      FATAL("ROM was shorter than expected.");
    }
    std::array<uint8_t, kBankSize.value()> memory_bank;
    program->read(reinterpret_cast<char *>(memory_bank.data()),
                  kBankSize.value());
    rom_banks_.push_back(memory_bank);
  }
}

void Cartridge::SetHardware(uint8_t byte) {
  switch (byte) {
    case 0x00:
      break;
    case 0x01:
      header_.hardware.controller_type = MemoryBankControllerType::kController1;
      [[fallthrough]];
    case 0x02:
      header_.hardware.has_ram = true;
      [[fallthrough]];
    case 0x03:
      header_.hardware.has_battery = true;
      break;
    case 0x05:
      header_.hardware.controller_type = MemoryBankControllerType::kController2;
      [[fallthrough]];
    case 0x06:
      header_.hardware.has_battery = true;
      break;
    case 0x08:
      header_.hardware.has_ram = true;
      [[fallthrough]];
    case 0x09:
      header_.hardware.has_battery = true;
      break;
    case 0x0F:
      header_.hardware.controller_type = MemoryBankControllerType::kController3;
      header_.hardware.has_timer = true;
      header_.hardware.has_battery = true;
      break;
    case 0x11:
      header_.hardware.controller_type = MemoryBankControllerType::kController3;
      [[fallthrough]];
    case 0x12:
      header_.hardware.has_ram = true;
      [[fallthrough]];
    case 0x13:
      header_.hardware.has_battery = true;
      [[fallthrough]];
    case 0x10:
      header_.hardware.has_timer = true;
      break;
    case 0x19:
      header_.hardware.controller_type = MemoryBankControllerType::kController5;
      [[fallthrough]];
    case 0x1A:
      header_.hardware.has_ram = true;
      [[fallthrough]];
    case 0x1B:
      header_.hardware.has_battery = true;
      break;
    case 0x1C:
      header_.hardware.controller_type = MemoryBankControllerType::kController5;
      header_.hardware.has_rumble = true;
      [[fallthrough]];
    case 0x1D:
      header_.hardware.has_sram = true;
      [[fallthrough]];
    case 0x1E:
      header_.hardware.has_battery = true;
      break;
    default:
      FATAL("Unknown cartridge type %x", byte);
  }
}

}  // namespace gamebun
