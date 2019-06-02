#include "cartridge.h"
#include "emulator.h"
#include "util/logging.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

using ::gamebun::Cartridge;
using ::gamebun::Emulator;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " <cart_file>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream cart_file(argv[1], std::ios::binary);
  if (cart_file.fail()) {
    FATAL("Unable to open %s", argv[1]);
  }
  Cartridge cart(&cart_file);

  Emulator emu(cart);
  bool success = emu.Run();
  if (!success) {
    FATAL("Emulator returned an error");
  }

  return EXIT_SUCCESS;
}
