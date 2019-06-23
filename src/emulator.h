#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "cartridge.h"
#include "memory.h"

namespace gamebun {

class Emulator {
 public:
  explicit Emulator(const Cartridge& cart);

  bool Run();

 private:
  const Memory memory_;
};

}  // namespace gamebun

#endif  // EMULATOR_H_
