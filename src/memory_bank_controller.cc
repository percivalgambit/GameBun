#include "memory_bank_controller.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <variant>

#include "memory.h"
#include "util/logging.h"

namespace gamebun {

std::unique_ptr<MemoryBankController> MemoryBankController::SelectController(
    MemoryBankControllerType controller_type) {
  switch (controller_type) {
    case MemoryBankControllerType::kNone:
      return std::make_unique<NoController>();
    case MemoryBankControllerType::kController1:
      return std::make_unique<Controller1>();
    case MemoryBankControllerType::kController2:
      return std::make_unique<Controller2>();
    case MemoryBankControllerType::kController3:
      return std::make_unique<Controller3>();
    case MemoryBankControllerType::kController5:
      return std::make_unique<Controller5>();
    default:
      FATAL("Unknown memory bank type");
  }
}

// TODO: Implement Write for NoController
void NoController::Write(Address address __attribute__((unused)),
                         uint8_t value __attribute__((unused))) {}

size_t NoController::GetSelectedRomBank() const { return 1; }

// TODO: Implement GetSelectedRamBank for NoController
size_t NoController::GetSelectedRamBank() const { return 0; }

bool NoController::RamEnabled() const { return true; }

Controller1::Controller1()
    : memory_mode_(MemoryMode::k16MRom8KRam),
      selected_ram_bank_(0),
      selected_rom_bank_low_(1),
      selected_rom_bank_high_(0),
      ram_enabled_(false) {}

void Controller1::Write(Address address, uint8_t value) {
  if (address.value() < 0x2000) {
    ram_enabled_ = value & 0x0A;
  } else if (0x2000 <= address.value() && address.value() < 0x4000) {
    selected_rom_bank_low_ = value & 0x1F;
    if (selected_rom_bank_low_ == 0) {
      selected_rom_bank_low_ = 1;
    }
  } else if (0x4000 <= address.value() && address.value() < 0x6000) {
    switch (memory_mode_) {
      case MemoryMode::k4MRom32KRam:
        selected_ram_bank_ = value & 0x3;
        break;
      case MemoryMode::k16MRom8KRam:
        selected_rom_bank_high_ = value & 0x3;
        break;
    }
  } else if (0x6000 <= address.value() && address.value() < 0x8000) {
    memory_mode_ =
        value & 0x1 ? MemoryMode::k4MRom32KRam : MemoryMode::k16MRom8KRam;
  }
}

size_t Controller1::GetSelectedRomBank() const {
  return selected_rom_bank_low_ | selected_rom_bank_high_;
}

size_t Controller1::GetSelectedRamBank() const { return selected_ram_bank_; }

bool Controller1::RamEnabled() const { return ram_enabled_; }

Controller2::Controller2() : selected_rom_bank_(1), ram_enabled_(false) {}

void Controller2::Write(Address address, uint8_t value) {
  if (address.value() < 0x2000) {
    if (!(address.value() & 0x0100)) {
      ram_enabled_ = value & 0x0A;
    }
  } else if (0x2000 <= address.value() && address.value() < 0x4000) {
    if (address.value() & 0x0100) {
      selected_rom_bank_ = value & 0xF;
    }
  }
}

size_t Controller2::GetSelectedRomBank() const { return selected_rom_bank_; }

size_t Controller2::GetSelectedRamBank() const { return 0; }

bool Controller2::RamEnabled() const { return ram_enabled_; }

// TODO: Implement MBC3
void Controller3::Write(Address address __attribute__((unused)),
                        uint8_t value __attribute__((unused))) {}

size_t Controller3::GetSelectedRomBank() const { return 1; }

size_t Controller3::GetSelectedRamBank() const { return 0; }

bool Controller3::RamEnabled() const { return false; }

// TODO: Implement MBC5
void Controller5::Write(Address address __attribute__((unused)),
                        uint8_t value __attribute__((unused))) {}

size_t Controller5::GetSelectedRomBank() const { return 1; }

size_t Controller5::GetSelectedRamBank() const { return 0; }

bool Controller5::RamEnabled() const { return false; }

}  // namespace gamebun
