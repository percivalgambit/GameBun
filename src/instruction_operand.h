#ifndef INSTRUCTION_OPERAND_H_
#define INSTRUCTION_OPERAND_H_

#include "memory.h"
#include "util/strong_int.h"

#include <cstddef>
#include <type_traits>
#include <variant>

namespace gamebun {

enum class RegisterByteIndex {
  A,
  B,
  C,
  D,
  E,
  H,
  L,
};

enum class RegisterBytePairIndex {
  AF,
  BC,
  DE,
  HL,
  SP,
};

enum class FlagIndex {
  C,
  Z,
  NC,
  NZ,
};

struct AddressIndex {
  explicit AddressIndex(RegisterBytePairIndex idx) : idx(idx) {}

  RegisterBytePairIndex idx;
};

DEFINE_STRONG_INT_TYPE(ImmediateByte, uint8_t)
DEFINE_STRONG_INT_TYPE(ImmediateBytePair, uint16_t)

struct ByteOperand {
  ByteOperand(RegisterByteIndex value) : value(value) {}
  ByteOperand(AddressIndex value) : value(value) {}
  ByteOperand(Address value) : value(value) {}
  ByteOperand(ImmediateByte value) : value(value) {}

  std::variant<RegisterByteIndex, AddressIndex, Address, ImmediateByte> value;
};

struct BytePairOperand {
  BytePairOperand(RegisterBytePairIndex value) : value(value) {}
  BytePairOperand(ImmediateBytePair value) : value(value) {}

  std::variant<RegisterBytePairIndex, ImmediateBytePair> value;
};

struct BitOperand {
  BitOperand(FlagIndex value) : value(value) {}

  FlagIndex value;
};

using InstructionOperand =
    std::variant<std::monostate, ByteOperand, BytePairOperand, BitOperand>;

}  // namespace gamebun

#endif  // INSTRUCTION_OPERAND_H_
