#include "emulator.h"

#include "cartridge.h"

namespace gamebun {

Emulator::Emulator(const Cartridge& cart)
    : memory_(cart.rom_banks, cart.header.ram_bank_num,
              cart.header.hardware.controller_type),
      cpu_(&memory_) {}

bool Emulator::Run() {
  while (true) {
    cpu_.Step();
  }
  return true;
}

}  // namespace gamebun
