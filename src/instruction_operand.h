#ifndef INSTRUCTION_OPERAND_H_
#define INSTRUCTION_OPERAND_H_

#include "memory.h"

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

struct ImmediateByte {};
struct ImmediateBytePair {};
struct ImmediateByteAddress {};
struct ImmediateBytePairAddress {};

struct ByteOperand {
  ByteOperand(RegisterByteIndex value) : value(value) {}
  ByteOperand(Address value) : value(value) {}
  ByteOperand(AddressIndex value) : value(value) {}
  ByteOperand(ImmediateByte value) : value(value) {}
  ByteOperand(ImmediateByteAddress value) : value(value) {}
  ByteOperand(ImmediateBytePairAddress value) : value(value) {}

  size_t InstructionBytesConsumed() const {
    if (std::holds_alternative<ImmediateByte>(value) ||
        std::holds_alternative<ImmediateByteAddress>(value)) {
      return 1;
    } else if (std::holds_alternative<ImmediateBytePairAddress>(value)) {
      return 2;
    }
    return 0;
  }

  std::variant<RegisterByteIndex, Address, AddressIndex, ImmediateByte,
               ImmediateByteAddress, ImmediateBytePairAddress>
      value;
};

struct BytePairOperand {
  BytePairOperand(RegisterBytePairIndex value) : value(value) {}
  BytePairOperand(ImmediateBytePair value) : value(value) {}

  size_t InstructionBytesConsumed() const {
    if (std::holds_alternative<ImmediateBytePair>(value)) {
      return 2;
    }
    return 0;
  }

  std::variant<RegisterBytePairIndex, ImmediateBytePair> value;
};

struct BitOperand {
  BitOperand(FlagIndex value) : value(value) {}

  size_t InstructionBytesConsumed() const { return 0; }

  FlagIndex value;
};

using InstructionOperand =
    std::variant<std::monostate, ByteOperand, BytePairOperand, BitOperand>;

constexpr size_t InstructionBytesConsumed(const InstructionOperand& operand) {
  return std::visit(
      [](auto&& arg) -> size_t {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
          return 0;
        } else if constexpr (std::is_same_v<T, ByteOperand>) {
          return arg.InstructionBytesConsumed();
        } else if constexpr (std::is_same_v<T, BytePairOperand>) {
          return arg.InstructionBytesConsumed();
        } else if constexpr (std::is_same_v<T, BitOperand>) {
          return arg.InstructionBytesConsumed();
        }
      },
      operand);
}

}  // namespace gamebun

#endif  // INSTRUCTION_OPERAND_H_
