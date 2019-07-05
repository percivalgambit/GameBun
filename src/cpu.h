#ifndef CPU_H_
#define CPU_H_

#include <cstdint>

#include "instruction_operand.h"
#include "memory.h"
#include "registers.h"

namespace gamebun {

class Cpu {
 public:
  Cpu(Memory* memory);

  void Step();

 private:
  uint8_t GetRegisterByte(RegisterByteIndex idx) const;
  uint16_t GetRegisterBytePair(RegisterBytePairIndex idx) const;

  uint8_t GetByte(const ByteOperand& operand) const;
  void SetByte(const ByteOperand& operand, uint8_t value);

  uint16_t GetBytePair(const BytePairOperand& operand) const;
  void SetBytePair(const BytePairOperand& operand, uint16_t value);

  bool GetBit(const BitOperand& operand) const;

  Memory* memory_;
  Registers registers_;
};

}  // namespace gamebun

#endif  // CPU_H_
