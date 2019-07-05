#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "instruction_operand.h"

#include <array>
#include <cstdint>

namespace gamebun {

enum class Opcode {
  LD,
  LDI,
  LDD,
  LDH,
  LDHL,

  ADD,
  ADC,
  SUB,
  SBC,
  AND,
  XOR,
  OR,
  CP,

  INC,
  DEC,

  RLCA,
  RRCA,
  RLA,
  RRA,
  DAA,
  CPL,
  SCF,
  CCF,

  JR,
  JP,

  POP,
  PUSH,

  RET,
  RETI,
  RST,
  CALL,

  DI,
  EI,

  NOP,
  STOP,
  HALT,
};

class Instruction {
 public:
  static Instruction Decode(const std::array<uint8_t, 3>& instruction_bytes);

 private:
  Instruction(Opcode op);
  Instruction(Opcode op, InstructionOperand operand1);
  Instruction(Opcode op, InstructionOperand operand1,
              InstructionOperand operand2);

  Opcode op_;
  InstructionOperand operand1_;
  InstructionOperand operand2_;
};

}  // namespace gamebun

#endif  // INSTRUCTION_H_
