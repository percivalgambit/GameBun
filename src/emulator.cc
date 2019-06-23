#include "emulator.h"

#include "cartridge.h"

namespace gamebun {

Emulator::Emulator(const Cartridge& cart)
    : memory_(cart.rom_banks, cart.header.ram_bank_num,
              cart.header.hardware.controller_type) {}

bool Emulator::Run() { return true; }

}  // namespace gamebun
