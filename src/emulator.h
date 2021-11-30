#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "cartridge.h"
#include "cpu.h"
#include "memory.h"

namespace gamebun {

class Emulator {
 public:
  explicit Emulator(const Cartridge& cart);

  bool Run();

  Emulator(const Emulator&) = delete;
  Emulator& operator=(const Emulator&) = delete;

 private:
  Memory memory_;
  Cpu cpu_;
};

}  // namespace gamebun

#endif  // EMULATOR_H_
