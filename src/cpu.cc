#include "cpu.h"

#include "instruction_operand.h"
#include "memory.h"
#include "registers.h"
#include "util/logging.h"

#include <cstdint>
#include <type_traits>
#include <variant>

namespace gamebun {

Cpu::Cpu(Memory* memory) : memory_(memory) {}

void Cpu::Step() {}

uint8_t Cpu::GetRegisterByte(RegisterByteIndex idx) const {
  switch (idx) {
    case RegisterByteIndex::A:
      return registers_.A();
    case RegisterByteIndex::B:
      return registers_.B();
    case RegisterByteIndex::C:
      return registers_.C();
    case RegisterByteIndex::D:
      return registers_.D();
    case RegisterByteIndex::E:
      return registers_.E();
    case RegisterByteIndex::H:
      return registers_.H();
    case RegisterByteIndex::L:
      return registers_.L();
  }
  FATAL("Unknown byte index %x", static_cast<uint8_t>(idx));
}

uint16_t Cpu::GetRegisterBytePair(RegisterBytePairIndex idx) const {
  switch (idx) {
    case RegisterBytePairIndex::AF:
      return registers_.AF();
    case RegisterBytePairIndex::BC:
      return registers_.BC();
    case RegisterBytePairIndex::DE:
      return registers_.DE();
    case RegisterBytePairIndex::HL:
      return registers_.HL();
    case RegisterBytePairIndex::SP:
      return registers_.SP();
  }
  FATAL("Unknown byte pair index %x", static_cast<uint8_t>(idx));
}

uint8_t Cpu::GetByte(const ByteOperand& operand) const {
  return std::visit(
      [*this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterByteIndex>) {
          return GetRegisterByte(arg);
        } else if constexpr (std::is_same_v<T, Address>) {
          return memory_->Read(arg);
        } else if constexpr (std::is_same_v<T, AddressIndex>) {
          return memory_->Read(Address(GetRegisterBytePair(arg.idx)));
        } else if constexpr (std::is_same_v<T, ImmediateByte>) {
          return arg.value();
        }
      },
      operand.value);
}

void Cpu::SetByte(const ByteOperand& operand, uint8_t value) {
  std::visit(
      [this, value](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterByteIndex>) {
          switch (arg) {
            case RegisterByteIndex::A:
              registers_.A() = value;
              break;
            case RegisterByteIndex::B:
              registers_.B() = value;
              break;
            case RegisterByteIndex::C:
              registers_.C() = value;
              break;
            case RegisterByteIndex::D:
              registers_.D() = value;
              break;
            case RegisterByteIndex::E:
              registers_.E() = value;
              break;
            case RegisterByteIndex::H:
              registers_.H() = value;
              break;
            case RegisterByteIndex::L:
              registers_.L() = value;
              break;
          }
        } else if constexpr (std::is_same_v<T, Address>) {
          memory_->Write(arg, value);
        } else if constexpr (std::is_same_v<T, AddressIndex>) {
          memory_->Write(Address(GetRegisterBytePair(arg.idx)), value);
        } else if constexpr (std::is_same_v<T, ImmediateByte>) {
          FATAL("Attempted to set the value of an immediate byte operand");
        }
      },
      operand.value);
}

uint16_t Cpu::GetBytePair(const BytePairOperand& operand) const {
  return std::visit(
      [*this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterBytePairIndex>) {
          return GetRegisterBytePair(arg);
        } else if constexpr (std::is_same_v<T, ImmediateBytePair>) {
          return arg.value();
        }
      },
      operand.value);
}

void Cpu::SetBytePair(const BytePairOperand& operand, uint16_t value) {
  return std::visit(
      [this, value](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterBytePairIndex>) {
          switch (arg) {
            case RegisterBytePairIndex::AF:
              registers_.AF() = value;
              break;
            case RegisterBytePairIndex::BC:
              registers_.BC() = value;
              break;
            case RegisterBytePairIndex::DE:
              registers_.DE() = value;
              break;
            case RegisterBytePairIndex::HL:
              registers_.HL() = value;
              break;
            case RegisterBytePairIndex::SP:
              registers_.SP() = value;
              break;
          }
        } else if constexpr (std::is_same_v<T, ImmediateBytePair>) {
          FATAL("Attempted to set the value of an immediate byte pair operand");
        }
      },
      operand.value);
}

bool Cpu::GetBit(const BitOperand& operand) const {
  switch (operand.value) {
    case FlagIndex::C:
      return registers_.F().carry;
    case FlagIndex::Z:
      return registers_.F().zero;
    case FlagIndex::NC:
      return !registers_.F().carry;
    case FlagIndex::NZ:
      return !registers_.F().zero;
  }
  FATAL("Unknown flag index %x", static_cast<uint8_t>(operand.value));
}

}  // namespace gamebun
