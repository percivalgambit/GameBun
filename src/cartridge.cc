#include "cartridge.h"

#include "util/byte_size.h"
#include "util/logging.h"

#include <array>
#include <cstdint>
#include <istream>

namespace gamebun {

// TODO: Add validation of cartridges.
Cartridge::Cartridge(std::istream *program) {
  std::array<uint8_t, kRomBankSize.value()> header_bank;
  program->read(reinterpret_cast<char *>(header_bank.data()),
                kRomBankSize.value());

  header.title =
      std::string_view(reinterpret_cast<char *>(&header_bank[0x134]), 16);
  header.color_gb = header_bank[0x143] == 0x80;
  header.super_gb = header_bank[0x146] == 0x03;

  const uint8_t hardware_spec = header_bank[0x147];
  switch (hardware_spec) {
    case 0x00:
      break;
    case 0x01:
      header.hardware.controller_type = MemoryBankControllerType::kController1;
      [[fallthrough]];
    case 0x02:
      header.hardware.has_ram = true;
      [[fallthrough]];
    case 0x03:
      header.hardware.has_battery = true;
      break;
    case 0x05:
      header.hardware.controller_type = MemoryBankControllerType::kController2;
      [[fallthrough]];
    case 0x06:
      header.hardware.has_battery = true;
      break;
    case 0x08:
      header.hardware.has_ram = true;
      [[fallthrough]];
    case 0x09:
      header.hardware.has_battery = true;
      break;
    case 0x0F:
      header.hardware.controller_type = MemoryBankControllerType::kController3;
      header.hardware.has_timer = true;
      header.hardware.has_battery = true;
      break;
    case 0x11:
      header.hardware.controller_type = MemoryBankControllerType::kController3;
      [[fallthrough]];
    case 0x12:
      header.hardware.has_ram = true;
      [[fallthrough]];
    case 0x13:
      header.hardware.has_battery = true;
      [[fallthrough]];
    case 0x10:
      header.hardware.has_timer = true;
      break;
    case 0x19:
      header.hardware.controller_type = MemoryBankControllerType::kController5;
      [[fallthrough]];
    case 0x1A:
      header.hardware.has_ram = true;
      [[fallthrough]];
    case 0x1B:
      header.hardware.has_battery = true;
      break;
    case 0x1C:
      header.hardware.controller_type = MemoryBankControllerType::kController5;
      header.hardware.has_rumble = true;
      [[fallthrough]];
    case 0x1D:
      header.hardware.has_sram = true;
      [[fallthrough]];
    case 0x1E:
      header.hardware.has_battery = true;
      break;
    default:
      FATAL("Unknown cartridge type %x", hardware_spec);
  }

  const uint8_t rom_size_spec = header_bank[0x148];
  header.rom_bank_num = 2;
  switch (rom_size_spec) {
    case 0x06:
      header.rom_bank_num *= 2;
      [[fallthrough]];
    case 0x05:
      header.rom_bank_num *= 2;
      [[fallthrough]];
    case 0x04:
      header.rom_bank_num *= 2;
      [[fallthrough]];
    case 0x03:
      header.rom_bank_num *= 2;
      [[fallthrough]];
    case 0x02:
      header.rom_bank_num *= 2;
      [[fallthrough]];
    case 0x01:
      header.rom_bank_num *= 2;
      [[fallthrough]];
    case 0x00:
      break;
    case 0x52:
      header.rom_bank_num = 72;
      break;
    case 0x53:
      header.rom_bank_num = 80;
      break;
    case 0x54:
      header.rom_bank_num = 96;
      break;
    default:
      FATAL("Invalid ROM size specifier: %x", rom_size_spec);
  }

  const uint8_t ram_size_spec = header_bank[0x149];
  switch (ram_size_spec) {
    case 0x00:
      break;
    case 0x01:
      header.ram_bank_num = 1;
      break;
    case 0x02:
      header.ram_bank_num = 1;
      break;
    case 0x03:
      header.ram_bank_num = 4;
      break;
    case 0x04:
      header.ram_bank_num = 16;
      break;
    default:
      FATAL("Invalid RAM size specifier: %x", ram_size_spec);
  }

  header.japanese_game = header_bank[0x14A] == 0x00;
  const uint8_t old_license_code = header_bank[0x14B];
  const uint16_t new_license_code =
      static_cast<uint16_t>((header_bank[0x144] << 8) | header_bank[0x145]);
  header.license_code =
      old_license_code == 0x33 ? new_license_code : old_license_code;
  header.super_gb &= old_license_code != 0x33;
  header.version = header_bank[0x14C];
  header.header_checksum = header_bank[0x14D];
  header.global_checksum =
      static_cast<uint16_t>((header_bank[0x14E] << 8) | header_bank[0x14F]);

  rom_banks.push_back(header_bank);
  for (unsigned i = 1; i < header.ram_bank_num; i++) {
    if (program->eof()) {
      FATAL("ROM was shorter than expected.");
    }
    std::array<uint8_t, kRomBankSize.value()> memory_bank;
    program->read(reinterpret_cast<char *>(memory_bank.data()),
                  kRomBankSize.value());
    rom_banks.push_back(memory_bank);
  }
}

}  // namespace gamebun
