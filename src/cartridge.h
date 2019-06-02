#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include <istream>

namespace gamebun {

class Cartridge {
 public:
  explicit Cartridge(std::istream *program);
};

}  // namespace gamebun

#endif  // CARTRIDGE_H_
