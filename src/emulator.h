#ifndef EMULATOR_H_
#define EMULATOR_H_

#include "cartridge.h"

namespace gamebun {

class Emulator {
 public:
  explicit Emulator(const Cartridge& cart);

  bool Run();
};

}  // namespace gamebun

#endif  // EMULATOR_H_
