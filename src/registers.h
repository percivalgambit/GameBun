#ifndef REGISTERS_H_
#define REGISTERS_H_

#include <cstdint>

namespace gamebun {

struct __attribute__((packed)) Flags {
  uint8_t : 4;
  bool carry : 1;
  bool half_carry : 1;
  bool subtract : 1;
  bool zero : 1;
};

class Registers {
 public:
  // TODO: Initialize registers correctly
  Registers() : sp_(0xFFFE), pc_(0x100) {}

  Flags F() const { return *reinterpret_cast<const Flags*>(&af_.parts.lo); }
  Flags& F() { return reinterpret_cast<Flags&>(af_.parts.lo); }

  uint8_t A() const { return af_.parts.hi; }
  uint8_t& A() { return af_.parts.hi; }
  uint8_t B() const { return bc_.parts.hi; }
  uint8_t& B() { return bc_.parts.hi; }
  uint8_t C() const { return bc_.parts.lo; }
  uint8_t& C() { return bc_.parts.lo; }
  uint8_t D() const { return de_.parts.hi; }
  uint8_t& D() { return de_.parts.hi; }
  uint8_t E() const { return de_.parts.lo; }
  uint8_t& E() { return de_.parts.lo; }
  uint8_t H() const { return hl_.parts.hi; }
  uint8_t& H() { return hl_.parts.hi; }
  uint8_t L() const { return hl_.parts.lo; }
  uint8_t& L() { return hl_.parts.lo; }

  uint16_t AF() const { return af_.full; }
  uint16_t& AF() { return af_.full; }
  uint16_t BC() const { return bc_.full; }
  uint16_t& BC() { return bc_.full; }
  uint16_t DE() const { return de_.full; }
  uint16_t& DE() { return de_.full; }
  uint16_t HL() const { return hl_.full; }
  uint16_t& HL() { return hl_.full; }
  uint16_t SP() const { return sp_; }
  uint16_t& SP() { return sp_; }
  uint16_t PC() const { return pc_; }
  uint16_t& PC() { return pc_; }

 private:
  struct RegisterPair {
    using Parts = struct {
      uint8_t lo;
      uint8_t hi;
    };
    union {
      Parts parts;
      uint16_t full;
    };
  };

  RegisterPair af_;
  RegisterPair bc_;
  RegisterPair de_;
  RegisterPair hl_;
  uint16_t sp_;
  uint16_t pc_;
};

}  // namespace gamebun

#endif  // REGISTERS_H_
