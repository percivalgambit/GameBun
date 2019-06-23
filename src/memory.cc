#include "memory.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "address.h"
#include "memory_bank_controller.h"
#include "util/logging.h"

namespace gamebun {

Memory::Memory(
    const std::vector<std::array<uint8_t, kRomBankSize.value()>>& rom_banks,
    size_t ram_bank_num, MemoryBankControllerType controller_type)
    : rom_banks_(rom_banks),
      ram_banks_(ram_bank_num),
      memory_bank_controller_(
          MemoryBankController::SelectController(controller_type)) {}

uint8_t Memory::Read(Address address) const {
  if (address.value() < 0x4000) {
    return rom_banks_[0][address.value()];
  } else if (0x4000 <= address.value() && address.value() < 0x8000) {
    const Address effective_addr = address - 0x4000;
    return rom_banks_[memory_bank_controller_->GetSelectedRomBank()]
                     [effective_addr.value()];
  } else if (0x8000 <= address.value() && address.value() < 0xA000) {
    // TODO: Implement reading from video RAM
  } else if (0xA000 <= address.value() && address.value() < 0xC000) {
    if (!memory_bank_controller_->RamEnabled()) {
      return 0;
    }

    const Address effective_addr = address - 0xA000;
    return ram_banks_[memory_bank_controller_->GetSelectedRamBank()]
                     [effective_addr.value()];
  } else if (0xC000 <= address.value() && address.value() < 0xE000) {
    // TODO: Implement reading from internal RAM
  } else if (0xE000 <= address.value() && address.value() < 0xFE00) {
    // TODO: Implement reading from internal RAM
  } else if (0xFE00 <= address.value() && address.value() < 0xFEA0) {
    // TODO: Implement reading from sprite attribute memory
  } else if (0xFEA0 <= address.value() && address.value() < 0xFF00) {
    // Empty, but unusable for I/O
    return 0;
  } else if (0xFF00 <= address.value() && address.value() < 0xFF4C) {
    // TODO: Implement reading from I/O ports
  } else if (0xFF4C <= address.value() && address.value() < 0xFF80) {
    // Empty, but unusable for I/O
    return 0;
  } else if (0xFF80 <= address.value() && address.value() < 0xFFFF) {
    // TODO: Implement reading from internal RAM
  } else if (address.value() == 0xFFFF) {
    // TODO: Implement reading from interrupt enable register
  }
  FATAL("Unexpected read address %x", address.value());
}

void Memory::Write(Address address, uint8_t value) {
  if (address.value() < 0x8000) {
    memory_bank_controller_->Write(address, value);
  } else if (0x8000 <= address.value() && address.value() < 0xA000) {
    // TODO: Implement writing to video RAM
  } else if (0xA000 <= address.value() && address.value() < 0xC000) {
    if (!memory_bank_controller_->RamEnabled()) {
      return;
    }

    const Address effective_addr = address - 0xA000;
    ram_banks_[memory_bank_controller_->GetSelectedRamBank()]
              [effective_addr.value()] = value;
  } else if (0xC000 <= address.value() && address.value() < 0xE000) {
    // TODO: Implement writing to internal RAM
  } else if (0xE000 <= address.value() && address.value() < 0xFE00) {
    // TODO: Implement writing to internal RAM
  } else if (0xFE00 <= address.value() && address.value() < 0xFEA0) {
    // TODO: Implement writing to sprite attribute memory
  } else if (0xFEA0 <= address.value() && address.value() < 0xFF00) {
    // Empty, but unusable for I/O
    return;
  } else if (0xFF00 <= address.value() && address.value() < 0xFF4C) {
    // TODO: Implement writing to I/O ports
  } else if (0xFF4C <= address.value() && address.value() < 0xFF80) {
    // Empty, but unusable for I/O
    return;
  } else if (0xFF80 <= address.value() && address.value() < 0xFFFF) {
    // TODO: Implement writing to internal RAM
  } else if (address.value() == 0xFFFF) {
    // TODO: Implement writing to interrupt enable register
  }
  FATAL("Unexpected write address %x", address.value());
}

}  // namespace gamebun
