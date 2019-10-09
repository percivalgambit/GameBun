#ifndef OPCODE_H_
#define OPCODE_H_

#include "instruction.h"
#include "instruction_operand.h"
#include "util/logging.h"

#include <cstddef>
#include <cstdint>

namespace gamebun {

struct InstructionPrefixEntry {
  constexpr explicit InstructionPrefixEntry(Instruction instruction)
      : instruction(instruction) {}

  Instruction instruction;
};

constexpr const InstructionPrefixEntry GetInstructionPrefixInfo(
    uint8_t instruction_prefix) {
  switch (instruction_prefix) {
    case 0x00:
      return InstructionPrefixEntry(Instruction(Opcode::NOP));
    case 0x01:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, RegisterBytePairIndex::BC, ImmediateBytePair()));
    case 0x02:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::BC),
                      RegisterByteIndex::A));
    case 0x03:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterBytePairIndex::BC));
    case 0x04:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::B));
    case 0x05:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::B));
    case 0x06:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, ImmediateByte()));
    case 0x07:
      return InstructionPrefixEntry(Instruction(Opcode::RLCA));
    case 0x08:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, ImmediateBytePairAddress(), RegisterBytePairIndex::SP));
    case 0x09:
      return InstructionPrefixEntry(Instruction(
          Opcode::ADD, RegisterBytePairIndex::HL, RegisterBytePairIndex::BC));
    case 0x0A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::BC)));
    case 0x0B:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterBytePairIndex::BC));
    case 0x0C:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::C));
    case 0x0D:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::C));
    case 0x0E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, ImmediateByte()));
    case 0x0F:
      return InstructionPrefixEntry(Instruction(Opcode::RRCA));
    case 0x10:
      return InstructionPrefixEntry(Instruction(Opcode::STOP));
    case 0x11:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, RegisterBytePairIndex::DE, ImmediateBytePair()));
    case 0x12:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::DE),
                      RegisterByteIndex::A));
    case 0x13:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterBytePairIndex::DE));
    case 0x14:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::D));
    case 0x15:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::D));
    case 0x16:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, ImmediateByte()));
    case 0x17:
      return InstructionPrefixEntry(Instruction(Opcode::RLA));
    case 0x18:
      return InstructionPrefixEntry(
          Instruction(Opcode::JR, ImmediateByteAddress()));
    case 0x19:
      return InstructionPrefixEntry(Instruction(
          Opcode::ADD, RegisterBytePairIndex::HL, RegisterBytePairIndex::DE));
    case 0x1A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::DE)));
    case 0x1B:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterBytePairIndex::DE));
    case 0x1C:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::E));
    case 0x1D:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::E));
    case 0x1E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, ImmediateByte()));
    case 0x1F:
      return InstructionPrefixEntry(Instruction(Opcode::RRA));
    case 0x20:
      return InstructionPrefixEntry(
          Instruction(Opcode::JR, FlagIndex::NZ, ImmediateByteAddress()));
    case 0x21:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, RegisterBytePairIndex::HL, ImmediateBytePair()));
    case 0x22:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDI, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::A));
    case 0x23:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterBytePairIndex::HL));
    case 0x24:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::H));
    case 0x25:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::H));
    case 0x26:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, ImmediateByte()));
    case 0x27:
      return InstructionPrefixEntry(Instruction(Opcode::DAA));
    case 0x28:
      return InstructionPrefixEntry(
          Instruction(Opcode::JR, FlagIndex::Z, ImmediateByteAddress()));
    case 0x29:
      return InstructionPrefixEntry(Instruction(
          Opcode::ADD, RegisterBytePairIndex::HL, RegisterBytePairIndex::HL));
    case 0x2A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDI, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x2B:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterBytePairIndex::HL));
    case 0x2C:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::L));
    case 0x2D:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::L));
    case 0x2E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, ImmediateByte()));
    case 0x2F:
      return InstructionPrefixEntry(Instruction(Opcode::CPL));
    case 0x30:
      return InstructionPrefixEntry(
          Instruction(Opcode::JR, FlagIndex::NC, ImmediateByteAddress()));
    case 0x31:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, RegisterBytePairIndex::SP, ImmediateBytePair()));
    case 0x32:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::A));
    case 0x33:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterBytePairIndex::SP));
    case 0x34:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, AddressIndex(RegisterBytePairIndex::HL)));
    case 0x35:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, AddressIndex(RegisterBytePairIndex::HL)));
    case 0x36:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      ImmediateByte()));
    case 0x37:
      return InstructionPrefixEntry(Instruction(Opcode::SCF));
    case 0x38:
      return InstructionPrefixEntry(
          Instruction(Opcode::JR, FlagIndex::C, ImmediateByteAddress()));
    case 0x39:
      return InstructionPrefixEntry(Instruction(
          Opcode::ADD, RegisterBytePairIndex::HL, RegisterBytePairIndex::SP));
    case 0x3A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDD, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x3B:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterBytePairIndex::SP));
    case 0x3C:
      return InstructionPrefixEntry(
          Instruction(Opcode::INC, RegisterByteIndex::A));
    case 0x3D:
      return InstructionPrefixEntry(
          Instruction(Opcode::DEC, RegisterByteIndex::A));
    case 0x3E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, ImmediateByte()));
    case 0x3F:
      return InstructionPrefixEntry(Instruction(Opcode::CCF));
    case 0x40:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::B));
    case 0x41:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::C));
    case 0x42:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::D));
    case 0x43:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::E));
    case 0x44:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::H));
    case 0x45:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::L));
    case 0x46:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x47:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::B, RegisterByteIndex::A));
    case 0x48:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::B));
    case 0x49:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::C));
    case 0x4A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::D));
    case 0x4B:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::E));
    case 0x4C:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::H));
    case 0x4D:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::L));
    case 0x4E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x4F:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::C, RegisterByteIndex::A));
    case 0x50:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::B));
    case 0x51:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::C));
    case 0x52:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::D));
    case 0x53:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::E));
    case 0x54:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::H));
    case 0x55:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::L));
    case 0x56:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x57:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::D, RegisterByteIndex::A));
    case 0x58:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::B));
    case 0x59:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::C));
    case 0x5A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::D));
    case 0x5B:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::E));
    case 0x5C:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::H));
    case 0x5D:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::L));
    case 0x5E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x5F:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::E, RegisterByteIndex::A));
    case 0x60:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::B));
    case 0x61:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::C));
    case 0x62:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::D));
    case 0x63:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::E));
    case 0x64:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::H));
    case 0x65:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::L));
    case 0x66:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x67:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::H, RegisterByteIndex::A));
    case 0x68:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::B));
    case 0x69:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::C));
    case 0x6A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::D));
    case 0x6B:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::E));
    case 0x6C:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::H));
    case 0x6D:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::L));
    case 0x6E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x6F:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::L, RegisterByteIndex::A));
    case 0x70:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::B));
    case 0x71:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::C));
    case 0x72:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::D));
    case 0x73:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::E));
    case 0x74:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::H));
    case 0x75:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                      RegisterByteIndex::L));
    case 0x76:
      return InstructionPrefixEntry(Instruction(Opcode::HALT));
    case 0x78:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0x79:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0x7A:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0x7B:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0x7C:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0x7D:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0x7E:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x7F:
      return InstructionPrefixEntry(
          Instruction(Opcode::LD, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0x80:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0x81:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0x82:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0x83:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0x84:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0x85:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0x86:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x87:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0x88:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0x89:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0x8A:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0x8B:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0x8C:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0x8D:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0x8E:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x8F:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0x90:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0x91:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0x92:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0x93:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0x94:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0x95:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0x96:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x97:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0x98:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0x99:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0x9A:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0x9B:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0x9C:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0x9D:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0x9E:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0x9F:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0xA0:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0xA1:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0xA2:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0xA3:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0xA4:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0xA5:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0xA6:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0xA7:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0xA8:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0xA9:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0xAA:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0xAB:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0xAC:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0xAD:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0xAE:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0xAF:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0xB0:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0xB1:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0xB2:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0xB3:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0xB4:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0xB5:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0xB6:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0xB7:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0xB8:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::B));
    case 0xB9:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0xBA:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::D));
    case 0xBB:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::E));
    case 0xBC:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::H));
    case 0xBD:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::L));
    case 0xBE:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A,
                      AddressIndex(RegisterBytePairIndex::HL)));
    case 0xBF:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, RegisterByteIndex::A));
    case 0xC0:
      return InstructionPrefixEntry(Instruction(Opcode::RET, FlagIndex::NZ));
    case 0xC1:
      return InstructionPrefixEntry(
          Instruction(Opcode::POP, RegisterBytePairIndex::BC));
    case 0xC2:
      return InstructionPrefixEntry(
          Instruction(Opcode::JP, FlagIndex::NZ, ImmediateBytePairAddress()));
    case 0xC3:
      return InstructionPrefixEntry(
          Instruction(Opcode::JP, ImmediateBytePairAddress()));
    case 0xC4:
      return InstructionPrefixEntry(
          Instruction(Opcode::CALL, FlagIndex::NZ, ImmediateBytePairAddress()));
    case 0xC5:
      return InstructionPrefixEntry(
          Instruction(Opcode::PUSH, RegisterBytePairIndex::BC));
    case 0xC6:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterByteIndex::A, ImmediateByte()));
    case 0xC7:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x00)));
    case 0xC8:
      return InstructionPrefixEntry(Instruction(Opcode::RET, FlagIndex::Z));
    case 0xC9:
      return InstructionPrefixEntry(Instruction(Opcode::RET));
    case 0xCA:
      return InstructionPrefixEntry(
          Instruction(Opcode::JP, FlagIndex::Z, ImmediateBytePairAddress()));
    case 0xCC:
      return InstructionPrefixEntry(
          Instruction(Opcode::CALL, FlagIndex::Z, ImmediateBytePairAddress()));
    case 0xCD:
      return InstructionPrefixEntry(
          Instruction(Opcode::CALL, ImmediateBytePairAddress()));
    case 0xCE:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADC, RegisterByteIndex::A, ImmediateByte()));
    case 0xCF:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x08)));
    case 0xD0:
      return InstructionPrefixEntry(Instruction(Opcode::RET, FlagIndex::NC));
    case 0xD1:
      return InstructionPrefixEntry(
          Instruction(Opcode::POP, RegisterBytePairIndex::DE));
    case 0xD2:
      return InstructionPrefixEntry(
          Instruction(Opcode::JP, FlagIndex::NC, ImmediateBytePairAddress()));
    case 0xD4:
      return InstructionPrefixEntry(
          Instruction(Opcode::CALL, FlagIndex::NC, ImmediateBytePairAddress()));
    case 0xD5:
      return InstructionPrefixEntry(
          Instruction(Opcode::PUSH, RegisterBytePairIndex::DE));
    case 0xD6:
      return InstructionPrefixEntry(
          Instruction(Opcode::SUB, RegisterByteIndex::A, ImmediateByte()));
    case 0xD7:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x10)));
    case 0xD8:
      return InstructionPrefixEntry(Instruction(Opcode::RET, FlagIndex::C));
    case 0xD9:
      return InstructionPrefixEntry(Instruction(Opcode::RETI));
    case 0xDA:
      return InstructionPrefixEntry(
          Instruction(Opcode::JP, FlagIndex::C, ImmediateBytePairAddress()));
    case 0xDC:
      return InstructionPrefixEntry(
          Instruction(Opcode::CALL, FlagIndex::C, ImmediateBytePairAddress()));
    case 0xDE:
      return InstructionPrefixEntry(
          Instruction(Opcode::SBC, RegisterByteIndex::A, ImmediateByte()));
    case 0xDF:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x18)));
    case 0xE0:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDH, ImmediateByte(), RegisterByteIndex::A));
    case 0xE1:
      return InstructionPrefixEntry(
          Instruction(Opcode::POP, RegisterBytePairIndex::HL));
    case 0xE2:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDH, RegisterByteIndex::C, RegisterByteIndex::A));
    case 0xE5:
      return InstructionPrefixEntry(
          Instruction(Opcode::PUSH, RegisterBytePairIndex::HL));
    case 0xE6:
      return InstructionPrefixEntry(
          Instruction(Opcode::AND, RegisterByteIndex::A, ImmediateByte()));
    case 0xE7:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x20)));
    case 0xE8:
      return InstructionPrefixEntry(
          Instruction(Opcode::ADD, RegisterBytePairIndex::SP, ImmediateByte()));
    case 0xE9:
      return InstructionPrefixEntry(
          Instruction(Opcode::JP, AddressIndex(RegisterBytePairIndex::HL)));
    case 0xEA:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, ImmediateBytePairAddress(), RegisterByteIndex::A));
    case 0xEE:
      return InstructionPrefixEntry(
          Instruction(Opcode::XOR, RegisterByteIndex::A, ImmediateByte()));
    case 0xEF:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x28)));
    case 0xF0:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDH, RegisterByteIndex::A, ImmediateByte()));
    case 0xF1:
      return InstructionPrefixEntry(
          Instruction(Opcode::POP, RegisterBytePairIndex::AF));
    case 0xF2:
      return InstructionPrefixEntry(
          Instruction(Opcode::LDH, RegisterByteIndex::A, RegisterByteIndex::C));
    case 0xF3:
      return InstructionPrefixEntry(Instruction(Opcode::DI));
    case 0xF5:
      return InstructionPrefixEntry(
          Instruction(Opcode::PUSH, RegisterBytePairIndex::AF));
    case 0xF6:
      return InstructionPrefixEntry(
          Instruction(Opcode::OR, RegisterByteIndex::A, ImmediateByte()));
    case 0xF7:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x30)));
    case 0xF8:
      return InstructionPrefixEntry(Instruction(
          Opcode::LDHL, RegisterBytePairIndex::SP, ImmediateByte()));
    case 0xF9:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, RegisterBytePairIndex::SP, RegisterBytePairIndex::HL));
    case 0xFA:
      return InstructionPrefixEntry(Instruction(
          Opcode::LD, RegisterByteIndex::A, ImmediateBytePairAddress()));
    case 0xFB:
      return InstructionPrefixEntry(Instruction(Opcode::EI));
    case 0xFE:
      return InstructionPrefixEntry(
          Instruction(Opcode::CP, RegisterByteIndex::A, ImmediateByte()));
    case 0xFF:
      return InstructionPrefixEntry(Instruction(Opcode::RST, Address(0x38)));
  }
  FATAL("Could not decode byte %x into an instruction header",
        instruction_prefix);
}

}  // namespace gamebun

#endif  // OPCODE_H_
