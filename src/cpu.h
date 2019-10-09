#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

#include "instruction.h"
#include "instruction_operand.h"
#include "memory.h"
#include "registers.h"

namespace gamebun {

class Cpu {
 public:
  Cpu(Memory* memory);

  void Step();

 private:
  Instruction FetchInstruction();
  void ExecuteInstruction(const Instruction& instruction);

  void AddByte(const ByteOperand dest, const uint8_t src, bool carry);
  void AddBytePair(const BytePairOperand dest, const uint16_t src);
  void AddBytePairUnsigned(const BytePairOperand dest, const uint16_t src);
  void SubByte(const ByteOperand dest, const uint8_t src, bool carry);
  void RotateByteLeft(const ByteOperand dest, bool through_carry);
  void RotateByteRight(const ByteOperand dest, bool through_carry);
  void ShiftByteLeft(const ByteOperand dest);
  void ShiftByteRight(const ByteOperand dest, bool arithmetic_shift);

  uint8_t GetRegisterByte(RegisterByteIndex idx) const;
  void SetRegisterByte(RegisterByteIndex idx, uint8_t value);

  uint16_t GetRegisterBytePair(RegisterBytePairIndex idx) const;
  void SetRegisterBytePair(RegisterBytePairIndex idx, uint16_t value);

  uint8_t GetByte(const ByteOperand& operand) const;
  void SetByte(const ByteOperand& operand, uint8_t value);

  uint16_t GetBytePair(const BytePairOperand& operand) const;
  void SetBytePair(const BytePairOperand& operand, uint16_t value);

  bool GetBit(const BitOperand& operand) const;

  Memory& memory_;
  Registers registers_;
};

}  // namespace gamebun

#endif  // CPU_H_
