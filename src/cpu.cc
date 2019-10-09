#include "cpu.h"

#include "instruction.h"
#include "instruction_operand.h"
#include "instruction_prefix.h"
#include "memory.h"
#include "registers.h"
#include "util/logging.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <variant>

namespace gamebun {

Cpu::Cpu(Memory* memory) : memory_(*memory) {}

void Cpu::Step() {
  const Instruction& instruction = FetchInstruction();
  ExecuteInstruction(instruction);
}

Instruction Cpu::FetchInstruction() {
  const uint8_t instruction_prefix = memory_.Read(Address(registers_.PC()));
  const Instruction& instruction =
      GetInstructionPrefixInfo(instruction_prefix).instruction;

  const size_t instruction_size =
      InstructionBytesConsumed(instruction.operand1) +
      InstructionBytesConsumed(instruction.operand2);
  registers_.PC() += instruction_size;

  return instruction;
}

void Cpu::ExecuteInstruction(const Instruction& instruction) {
  switch (instruction.op) {
    case Opcode::LD: {
      if (std::holds_alternative<ByteOperand>(instruction.operand1) &&
          std::holds_alternative<ByteOperand>(instruction.operand2)) {
        const ByteOperand& operand1 =
            std::get<ByteOperand>(instruction.operand1);
        const ByteOperand& operand2 =
            std::get<ByteOperand>(instruction.operand2);
        SetByte(operand1, GetByte(operand2));
      } else if (std::holds_alternative<BytePairOperand>(
                     instruction.operand1) &&
                 std::holds_alternative<BytePairOperand>(
                     instruction.operand2)) {
        const BytePairOperand& operand1 =
            std::get<BytePairOperand>(instruction.operand1);
        const BytePairOperand& operand2 =
            std::get<BytePairOperand>(instruction.operand2);
        SetBytePair(operand1, GetBytePair(operand2));
      } else {
        FATAL("Invalid operands for LD");
      }
      break;
    }
    case Opcode::LDI: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      SetByte(operand1, GetByte(operand2));

      assert(std::holds_alternative<AddressIndex>(operand1.value) ||
             std::holds_alternative<AddressIndex>(operand1.value));
      const RegisterBytePairIndex idx =
          std::holds_alternative<AddressIndex>(operand1.value)
              ? std::get<AddressIndex>(operand1.value).idx
              : std::get<AddressIndex>(operand2.value).idx;
      SetRegisterBytePair(idx, GetRegisterBytePair(idx) + 1);
      break;
    }
    case Opcode::LDD: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      SetByte(operand1, GetByte(operand2));

      assert(std::holds_alternative<AddressIndex>(operand1.value) ||
             std::holds_alternative<AddressIndex>(operand1.value));
      const RegisterBytePairIndex idx =
          std::holds_alternative<AddressIndex>(operand1.value)
              ? std::get<AddressIndex>(operand1.value).idx
              : std::get<AddressIndex>(operand2.value).idx;
      SetRegisterBytePair(idx, GetRegisterBytePair(idx) - 1);
      break;
    }
    case Opcode::LDH: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      if (std::holds_alternative<ImmediateByte>(operand1.value) ||
          (std::holds_alternative<RegisterByteIndex>(operand1.value) &&
           std::get<RegisterByteIndex>(operand1.value) ==
               RegisterByteIndex::C)) {
        const Address dst = Address(GetByte(operand1) + 0xFF00);
        memory_.Write(dst, GetByte(operand2));
      } else if (std::holds_alternative<ImmediateByte>(operand2.value) ||
                 (std::holds_alternative<RegisterByteIndex>(operand2.value) &&
                  std::get<RegisterByteIndex>(operand2.value) ==
                      RegisterByteIndex::C)) {
        const Address src = Address(GetByte(operand2) + 0xFF00);
        SetByte(operand1, memory_.Read(src));
      } else {
        FATAL("Invalid operands for LDH");
      }
      break;
    }
    case Opcode::LDHL: {
      const BytePairOperand& operand1 =
          std::get<BytePairOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      assert(std::get<RegisterBytePairIndex>(operand1.value) ==
             RegisterBytePairIndex::SP);

      registers_.HL() =
          GetRegisterBytePair(std::get<RegisterBytePairIndex>(operand1.value));
      AddBytePair(RegisterBytePairIndex::HL, GetByte(operand2));
      break;
    }
    case Opcode::PUSH: {
      const BytePairOperand& operand =
          std::get<BytePairOperand>(instruction.operand1);
      const uint16_t value = GetBytePair(operand);
      const Address sp = Address(registers_.SP());

      memory_.Write(sp, (value >> 8) & 0xFF);
      memory_.Write(sp + 1, value & 0xFF);
      registers_.SP() -= 2;
      break;
    }
    case Opcode::POP: {
      const BytePairOperand& operand =
          std::get<BytePairOperand>(instruction.operand1);
      const Address sp = Address(registers_.SP());

      const uint16_t value =
          static_cast<uint16_t>((memory_.Read(sp) << 8) | memory_.Read(sp + 1));
      SetBytePair(operand, value);
      registers_.SP() += 2;
      break;
    }
    case Opcode::ADD: {
      if (std::holds_alternative<ByteOperand>(instruction.operand1) &&
          std::holds_alternative<ByteOperand>(instruction.operand2)) {
        const ByteOperand& operand1 =
            std::get<ByteOperand>(instruction.operand1);
        const ByteOperand& operand2 =
            std::get<ByteOperand>(instruction.operand2);
        AddByte(operand1, GetByte(operand2), /*carry=*/false);
      } else if (std::holds_alternative<BytePairOperand>(
                     instruction.operand1)) {
        const BytePairOperand& operand1 =
            std::get<BytePairOperand>(instruction.operand1);
        if (std::holds_alternative<BytePairOperand>(instruction.operand2)) {
          const BytePairOperand& operand2 =
              std::get<BytePairOperand>(instruction.operand2);
          AddBytePairUnsigned(operand1, GetBytePair(operand2));
        } else if (std::holds_alternative<ByteOperand>(instruction.operand2)) {
          const ByteOperand& operand2 =
              std::get<ByteOperand>(instruction.operand2);
          AddBytePair(operand1, GetByte(operand2));
        } else {
          FATAL("invalid operands for ADD");
        }
      } else {
        FATAL("Invalid operands for ADD");
      }
      break;
    }
    case Opcode::ADC: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      AddByte(operand1, GetByte(operand2), /*carry=*/true);
      break;
    }
    case Opcode::SUB: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      SubByte(operand1, GetByte(operand2), /*carry=*/false);
      break;
    }
    case Opcode::SBC: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);
      SubByte(operand1, GetByte(operand2), /*carry=*/true);
      break;
    }
    case Opcode::AND: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);

      const uint8_t result = GetByte(operand1) & GetByte(operand2);
      SetByte(operand1, result);
      registers_.F().zero = result == 0;
      registers_.F().subtract = false;
      registers_.F().half_carry = true;
      registers_.F().carry = false;
      break;
    }
    case Opcode::OR: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);

      const uint8_t result = GetByte(operand1) | GetByte(operand2);
      SetByte(operand1, result);
      registers_.F().zero = result == 0;
      registers_.F().subtract = false;
      registers_.F().half_carry = false;
      registers_.F().carry = false;
      break;
    }
    case Opcode::XOR: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);

      const uint8_t result = GetByte(operand1) ^ GetByte(operand2);
      SetByte(operand1, result);
      registers_.F().zero = result == 0;
      registers_.F().subtract = false;
      registers_.F().half_carry = false;
      registers_.F().carry = false;
      break;
    }
    case Opcode::CP: {
      const ByteOperand& operand1 = std::get<ByteOperand>(instruction.operand1);
      const ByteOperand& operand2 = std::get<ByteOperand>(instruction.operand2);

      const uint8_t destVal = GetByte(operand1);
      SubByte(operand1, GetByte(operand2), /*carry=*/false);
      SetByte(operand1, destVal);
      break;
    }
    case Opcode::INC: {
      if (std::holds_alternative<ByteOperand>(instruction.operand1)) {
        const ByteOperand& operand =
            std::get<ByteOperand>(instruction.operand1);
        AddByte(operand, 1, /*carry=*/false);
      } else if (std::holds_alternative<BytePairOperand>(
                     instruction.operand1)) {
        const BytePairOperand& operand =
            std::get<BytePairOperand>(instruction.operand1);
        SetBytePair(operand, GetBytePair(operand) + 1);
      } else {
        FATAL("Invalid operands for INC");
      }
      break;
    }
    case Opcode::DEC: {
      if (std::holds_alternative<ByteOperand>(instruction.operand1)) {
        const ByteOperand& operand =
            std::get<ByteOperand>(instruction.operand1);
        SubByte(operand, 1, /*carry=*/false);
      } else if (std::holds_alternative<BytePairOperand>(
                     instruction.operand1)) {
        const BytePairOperand& operand =
            std::get<BytePairOperand>(instruction.operand1);
        SetBytePair(operand, GetBytePair(operand) - 1);
      } else {
        FATAL("Invalid operands for DEC");
      }
      break;
    }
    case Opcode::SWAP: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      const uint8_t val = GetByte(operand);
      const uint8_t lo = val & 0xF;
      const uint8_t hi = (val & 0xF0) >> 4;

      SetByte(operand, static_cast<uint8_t>((lo << 4) | hi));
      registers_.F().zero = val == 0;
      registers_.F().subtract = false;
      registers_.F().half_carry = false;
      registers_.F().carry = false;
      break;
    }
    case Opcode::DAA: {
      // TODO: implement DAA
      const uint8_t val = registers_.A();

      registers_.F().zero = val == 0;
      registers_.F().half_carry = false;
      break;
    }
    case Opcode::CPL: {
      const uint8_t val = registers_.A();

      registers_.A() = ~val;
      registers_.F().subtract = true;
      registers_.F().half_carry = true;
      break;
    }
    case Opcode::CCF: {
      registers_.F().subtract = false;
      registers_.F().half_carry = false;
      registers_.F().carry = !registers_.F().carry;
      break;
    }
    case Opcode::SCF: {
      registers_.F().subtract = false;
      registers_.F().half_carry = false;
      registers_.F().carry = true;
      break;
    }
    case Opcode::NOP:
      break;
    case Opcode::HALT:
      // TODO: implement HALT
      break;
    case Opcode::STOP:
      // TODO: implement STOP
      break;
    case Opcode::DI:
      // TODO: implement DI
      break;
    case Opcode::EI:
      // TODO: implement EI
      break;
    case Opcode::RLCA: {
      RotateByteLeft(RegisterByteIndex::A, /*through_carry=*/false);
      break;
    }
    case Opcode::RLA: {
      RotateByteLeft(RegisterByteIndex::A, /*through_carry=*/true);
      break;
    }
    case Opcode::RRCA: {
      RotateByteRight(RegisterByteIndex::A, /*through_carry=*/false);
      break;
    }
    case Opcode::RRA: {
      RotateByteRight(RegisterByteIndex::A, /*through_carry=*/true);
      break;
    }
    case Opcode::RLC: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      RotateByteLeft(std::get<RegisterByteIndex>(operand.value),
                     /*through_carry=*/false);
      break;
    }
    case Opcode::RL: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      RotateByteLeft(std::get<RegisterByteIndex>(operand.value),
                     /*through_carry=*/true);
      break;
    }
    case Opcode::RRC: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      RotateByteRight(std::get<RegisterByteIndex>(operand.value),
                      /*through_carry=*/false);
      break;
    }
    case Opcode::RR: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      RotateByteRight(std::get<RegisterByteIndex>(operand.value),
                      /*through_carry=*/true);
      break;
    }
    case Opcode::SLA: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      ShiftByteLeft(std::get<RegisterByteIndex>(operand.value));
      break;
    }
    case Opcode::SRA: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      ShiftByteRight(std::get<RegisterByteIndex>(operand.value),
                     /*arithmetic_shift=*/true);
      break;
    }
    case Opcode::SRL: {
      const ByteOperand& operand = std::get<ByteOperand>(instruction.operand1);
      ShiftByteRight(std::get<RegisterByteIndex>(operand.value),
                     /*arithmetic_shift=*/false);
      break;
    }
    case Opcode::BIT:
    case Opcode::SET:
    case Opcode::RES:
    case Opcode::JP:
    case Opcode::JR:
    case Opcode::CALL:
    case Opcode::RST:
    case Opcode::RET:
    case Opcode::RETI:
      // TODO: Implement the rest of the GB instructions
      FATAL("Opcode is not implemented");
    default:
      FATAL("Invalid opcode");
  }
}

void Cpu::AddByte(const ByteOperand dest, const uint8_t src, bool carry) {
  const uint8_t destVal = GetByte(dest);
  const int16_t carryVal = static_cast<int16_t>(carry && registers_.F().carry);
  const int16_t total =
      static_cast<int16_t>(destVal) + static_cast<int16_t>(src) + carryVal;

  SetByte(dest, static_cast<uint8_t>(total));
  registers_.F().zero = static_cast<uint8_t>(total) == 0;
  registers_.F().subtract = false;
  registers_.F().half_carry =
      (destVal & 0xF) + (src & 0xF) + static_cast<uint8_t>(carryVal) > 0xF;
  registers_.F().carry = total > 0xFF;
}

void Cpu::AddBytePair(const BytePairOperand dest, const uint16_t src) {
  const uint16_t destVal = GetBytePair(dest);
  const int32_t total =
      static_cast<int32_t>(destVal) + static_cast<int32_t>(src);
  const uint16_t carryBits = destVal ^ static_cast<uint16_t>(total) ^ src;

  SetBytePair(dest, static_cast<uint16_t>(total));
  registers_.F().zero = false;
  registers_.F().subtract = false;
  registers_.F().half_carry = (carryBits & 0x10) == 0x10;
  registers_.F().carry = (carryBits & 0x100) == 0x100;
}

void Cpu::AddBytePairUnsigned(const BytePairOperand dest, const uint16_t src) {
  const uint16_t destVal = GetBytePair(dest);
  const int32_t total =
      static_cast<int32_t>(destVal) + static_cast<int32_t>(src);

  SetBytePair(dest, static_cast<uint16_t>(total));
  registers_.F().subtract = false;
  registers_.F().half_carry = (destVal & 0xFFF) > (total & 0xFFF);
  registers_.F().carry = total > 0xFFFF;
}

void Cpu::SubByte(const ByteOperand dest, const uint8_t src, bool carry) {
  const uint8_t destVal = GetByte(dest);
  const int16_t carryVal = static_cast<int16_t>(carry && registers_.F().carry);
  const int16_t total =
      static_cast<int16_t>(destVal) - static_cast<int16_t>(src) - carryVal;

  SetByte(dest, static_cast<uint8_t>(total));
  registers_.F().zero = static_cast<uint8_t>(total) == 0;
  registers_.F().subtract = true;
  registers_.F().half_carry = static_cast<int16_t>(destVal & 0xF) -
                                  static_cast<int16_t>(src & 0xF) - carry <
                              0;
  registers_.F().carry = total < 0;
}

void Cpu::RotateByteLeft(const ByteOperand dest, bool through_carry) {
  const uint8_t val = GetByte(dest);
  const uint8_t carry =
      static_cast<uint8_t>(through_carry && registers_.F().carry);

  SetByte(dest, static_cast<uint8_t>(val << 1) | carry);
  registers_.F().zero = GetByte(dest) == 0;
  registers_.F().subtract = false;
  registers_.F().half_carry = false;
  registers_.F().carry = val & 0x80;
}

void Cpu::RotateByteRight(const ByteOperand dest, bool through_carry) {
  const uint8_t val = GetByte(dest);
  const uint8_t carry =
      static_cast<uint8_t>((through_carry && registers_.F().carry) << 7);

  SetByte(dest, static_cast<uint8_t>(val >> 1) | carry);
  registers_.F().zero = GetByte(dest) == 0;
  registers_.F().subtract = false;
  registers_.F().half_carry = false;
  registers_.F().carry = val & 0x1;
}

void Cpu::ShiftByteLeft(const ByteOperand dest) {
  const uint8_t val = GetByte(dest);

  SetByte(dest, static_cast<uint8_t>(val << 1));
  registers_.F().zero = GetByte(dest) == 0;
  registers_.F().subtract = false;
  registers_.F().half_carry = false;
  registers_.F().carry = val & 0x80;
}

void Cpu::ShiftByteRight(const ByteOperand dest, bool arithmetic_shift) {
  const uint8_t val = GetByte(dest);
  const uint8_t new_msb = arithmetic_shift ? val & 0x8 : 0;

  SetByte(dest, static_cast<uint8_t>(val >> 1) | new_msb);
  registers_.F().zero = GetByte(dest) == 0;
  registers_.F().subtract = false;
  registers_.F().half_carry = false;
  registers_.F().carry = val & 0x1;
}

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

void Cpu::SetRegisterByte(RegisterByteIndex idx, uint8_t value) {
  switch (idx) {
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

void Cpu::SetRegisterBytePair(RegisterBytePairIndex idx, uint16_t value) {
  switch (idx) {
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
}

uint8_t Cpu::GetByte(const ByteOperand& operand) const {
  return std::visit(
      [*this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterByteIndex>) {
          return GetRegisterByte(arg);
        } else if constexpr (std::is_same_v<T, Address>) {
          return memory_.Read(arg);
        } else if constexpr (std::is_same_v<T, AddressIndex>) {
          return memory_.Read(Address(GetRegisterBytePair(arg.idx)));
        } else if constexpr (std::is_same_v<T, ImmediateByte>) {
          return memory_.Read(Address(registers_.PC() + 1));
        } else if constexpr (std::is_same_v<T, ImmediateByteAddress>) {
          const Address address =
              Address(memory_.Read(Address(registers_.PC() + 1)));
          return memory_.Read(address);
        } else if constexpr (std::is_same_v<T, ImmediateBytePairAddress>) {
          const Address pc = Address(registers_.PC());
          const Address address = Address(static_cast<uint16_t>(
              (memory_.Read(pc + 1) << 8) | memory_.Read(pc + 2)));
          return memory_.Read(address);
        }
      },
      operand.value);
}

void Cpu::SetByte(const ByteOperand& operand, uint8_t value) {
  std::visit(
      [this, value](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterByteIndex>) {
          SetRegisterByte(arg, value);
        } else if constexpr (std::is_same_v<T, Address>) {
          memory_.Write(arg, value);
        } else if constexpr (std::is_same_v<T, AddressIndex>) {
          memory_.Write(Address(GetRegisterBytePair(arg.idx)), value);
        } else if constexpr (std::is_same_v<T, ImmediateByte>) {
          FATAL("Attempted to set the value of an immediate byte operand");
        } else if constexpr (std::is_same_v<T, ImmediateByteAddress>) {
          const Address address =
              Address(memory_.Read(Address(registers_.PC() + 1)));
          memory_.Write(address, value);
        } else if constexpr (std::is_same_v<T, ImmediateByte>) {
          const Address pc = Address(registers_.PC());
          const Address address = Address(static_cast<uint16_t>(
              (memory_.Read(pc + 1) << 8) | memory_.Read(pc + 2)));
          memory_.Write(address, value);
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
          const Address pc = Address(registers_.PC());
          return static_cast<uint16_t>((memory_.Read(pc + 1) << 8) |
                                       memory_.Read(pc + 2));
        }
      },
      operand.value);
}

void Cpu::SetBytePair(const BytePairOperand& operand, uint16_t value) {
  return std::visit(
      [this, value](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, RegisterBytePairIndex>) {
          SetRegisterBytePair(arg, value);
        } else if constexpr (std::is_same_v<T, ImmediateBytePair>) {
          FATAL("Attempted to set the value of an immediate byte pair operand");
        }
      },
      operand.value);
}  // namespace gamebun

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
