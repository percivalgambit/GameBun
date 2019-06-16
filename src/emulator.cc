#include "emulator.h"

#include "cartridge.h"

namespace gamebun {

Emulator::Emulator(const Cartridge& cart) : cart_(cart) {}

bool Emulator::Run() { return true; }

}  // namespace gamebun
