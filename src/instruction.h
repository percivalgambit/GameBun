#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "instruction_operand.h"

namespace gamebun {

// TODO: Add CB prefix instructions
enum class Opcode {
  LD,
  LDI,
  LDD,
  LDH,
  LDHL,

  PUSH,
  POP,

  ADD,
  ADC,
  SUB,
  SBC,
  AND,
  OR,
  XOR,
  CP,

  INC,
  DEC,

  SWAP,
  DAA,
  CPL,
  CCF,
  SCF,

  NOP,
  HALT,
  STOP,

  DI,
  EI,

  RLCA,
  RLA,
  RRCA,
  RRA,
  RLC,
  RL,
  RRC,
  RR,
  SLA,
  SRA,
  SRL,

  BIT,
  SET,
  RES,

  JP,
  JR,

  CALL,
  RST,
  RET,
  RETI,
};

struct Instruction {
  constexpr Instruction(Opcode op) : op(op) {}
  constexpr Instruction(Opcode op, InstructionOperand operand1)
      : op(op), operand1(operand1) {}
  constexpr Instruction(Opcode op, InstructionOperand operand1,
                        InstructionOperand operand2)
      : op(op), operand1(operand1), operand2(operand2) {}

  Opcode op;
  InstructionOperand operand1;
  InstructionOperand operand2;
};

}  // namespace gamebun

#endif  // INSTRUCTION_H_
