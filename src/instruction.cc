#include "instruction.h"

#include <cstdint>
#include <utility>

#include "address.h"
#include "instruction_operand.h"
#include "util/logging.h"

namespace gamebun {

Instruction Instruction::Decode(
    const std::array<uint8_t, 3>& instruction_bytes) {
  const ImmediateBytePair immediate_byte_pair =
      ImmediateBytePair((instruction_bytes[1] << 8) | instruction_bytes[2]);
  switch (instruction_bytes[0]) {
    case 0x00:
      return Instruction(Opcode::NOP);
    case 0x01:
      return Instruction(Opcode::LD, RegisterBytePairIndex::BC,
                         immediate_byte_pair);
    case 0x02:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::BC),
                         RegisterByteIndex::A);
    case 0x03:
      return Instruction(Opcode::INC, RegisterBytePairIndex::BC);
    case 0x04:
      return Instruction(Opcode::INC, RegisterByteIndex::B);
    case 0x05:
      return Instruction(Opcode::DEC, RegisterByteIndex::B);
    case 0x06:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         ImmediateByte(instruction_bytes[1]));
    case 0x07:
      return Instruction(Opcode::RLCA);
    case 0x08:
      return Instruction(Opcode::LD, Address(immediate_byte_pair.value()),
                         RegisterBytePairIndex::SP);
    case 0x09:
      return Instruction(Opcode::ADD, RegisterBytePairIndex::HL,
                         RegisterBytePairIndex::BC);
    case 0x0A:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::BC));
    case 0x0B:
      return Instruction(Opcode::DEC, RegisterBytePairIndex::BC);
    case 0x0C:
      return Instruction(Opcode::INC, RegisterByteIndex::C);
    case 0x0D:
      return Instruction(Opcode::DEC, RegisterByteIndex::C);
    case 0x0E:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         ImmediateByte(instruction_bytes[1]));
    case 0x0F:
      return Instruction(Opcode::RRCA);
    case 0x10:
      return Instruction(Opcode::STOP);
    case 0x11:
      return Instruction(Opcode::LD, RegisterBytePairIndex::DE,
                         immediate_byte_pair);
    case 0x12:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::DE),
                         RegisterByteIndex::A);
    case 0x13:
      return Instruction(Opcode::INC, RegisterBytePairIndex::DE);
    case 0x14:
      return Instruction(Opcode::INC, RegisterByteIndex::D);
    case 0x15:
      return Instruction(Opcode::DEC, RegisterByteIndex::D);
    case 0x16:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         ImmediateByte(instruction_bytes[1]));
    case 0x17:
      return Instruction(Opcode::RLA);
    case 0x18:
      return Instruction(Opcode::JR, Address(instruction_bytes[1]));
    case 0x19:
      return Instruction(Opcode::ADD, RegisterBytePairIndex::HL,
                         RegisterBytePairIndex::DE);
    case 0x1A:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::DE));
    case 0x1B:
      return Instruction(Opcode::DEC, RegisterBytePairIndex::DE);
    case 0x1C:
      return Instruction(Opcode::INC, RegisterByteIndex::E);
    case 0x1D:
      return Instruction(Opcode::DEC, RegisterByteIndex::E);
    case 0x1E:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         ImmediateByte(instruction_bytes[1]));
    case 0x1F:
      return Instruction(Opcode::RRA);
    case 0x20:
      return Instruction(Opcode::JR, FlagIndex::NZ,
                         Address(instruction_bytes[1]));
    case 0x21:
      return Instruction(Opcode::LD, RegisterBytePairIndex::HL,
                         immediate_byte_pair);
    case 0x22:
      return Instruction(Opcode::LDI, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::A);
    case 0x23:
      return Instruction(Opcode::INC, RegisterBytePairIndex::HL);
    case 0x24:
      return Instruction(Opcode::INC, RegisterByteIndex::H);
    case 0x25:
      return Instruction(Opcode::DEC, RegisterByteIndex::H);
    case 0x26:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         ImmediateByte(instruction_bytes[1]));
    case 0x27:
      return Instruction(Opcode::DAA);
    case 0x28:
      return Instruction(Opcode::JR, FlagIndex::Z,
                         Address(instruction_bytes[1]));
    case 0x29:
      return Instruction(Opcode::ADD, RegisterBytePairIndex::HL,
                         RegisterBytePairIndex::HL);
    case 0x2A:
      return Instruction(Opcode::LDI, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x2B:
      return Instruction(Opcode::DEC, RegisterBytePairIndex::HL);
    case 0x2C:
      return Instruction(Opcode::INC, RegisterByteIndex::L);
    case 0x2D:
      return Instruction(Opcode::DEC, RegisterByteIndex::L);
    case 0x2E:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         ImmediateByte(instruction_bytes[1]));
    case 0x2F:
      return Instruction(Opcode::CPL);
    case 0x30:
      return Instruction(Opcode::JR, FlagIndex::NC,
                         Address(instruction_bytes[1]));
    case 0x31:
      return Instruction(Opcode::LD, RegisterBytePairIndex::SP,
                         immediate_byte_pair);
    case 0x32:
      return Instruction(Opcode::LDD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::A);
    case 0x33:
      return Instruction(Opcode::INC, RegisterBytePairIndex::SP);
    case 0x34:
      return Instruction(Opcode::INC, AddressIndex(RegisterBytePairIndex::HL));
    case 0x35:
      return Instruction(Opcode::DEC, AddressIndex(RegisterBytePairIndex::HL));
    case 0x36:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         ImmediateByte(instruction_bytes[1]));
    case 0x37:
      return Instruction(Opcode::SCF);
    case 0x38:
      return Instruction(Opcode::JR, FlagIndex::C,
                         Address(instruction_bytes[1]));
    case 0x39:
      return Instruction(Opcode::ADD, RegisterBytePairIndex::HL,
                         RegisterBytePairIndex::SP);
    case 0x3A:
      return Instruction(Opcode::LDD, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x3B:
      return Instruction(Opcode::DEC, RegisterBytePairIndex::SP);
    case 0x3C:
      return Instruction(Opcode::INC, RegisterByteIndex::A);
    case 0x3D:
      return Instruction(Opcode::DEC, RegisterByteIndex::A);
    case 0x3E:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         ImmediateByte(instruction_bytes[1]));
    case 0x3F:
      return Instruction(Opcode::CCF);
    case 0x40:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::B);
    case 0x41:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::C);
    case 0x42:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::D);
    case 0x43:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::E);
    case 0x44:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::H);
    case 0x45:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::L);
    case 0x46:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x47:
      return Instruction(Opcode::LD, RegisterByteIndex::B,
                         RegisterByteIndex::A);
    case 0x48:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::B);
    case 0x49:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::C);
    case 0x4A:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::D);
    case 0x4B:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::E);
    case 0x4C:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::H);
    case 0x4D:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::L);
    case 0x4E:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x4F:
      return Instruction(Opcode::LD, RegisterByteIndex::C,
                         RegisterByteIndex::A);
    case 0x50:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::B);
    case 0x51:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::C);
    case 0x52:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::D);
    case 0x53:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::E);
    case 0x54:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::H);
    case 0x55:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::L);
    case 0x56:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x57:
      return Instruction(Opcode::LD, RegisterByteIndex::D,
                         RegisterByteIndex::A);
    case 0x58:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::B);
    case 0x59:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::C);
    case 0x5A:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::D);
    case 0x5B:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::E);
    case 0x5C:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::H);
    case 0x5D:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::L);
    case 0x5E:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x5F:
      return Instruction(Opcode::LD, RegisterByteIndex::E,
                         RegisterByteIndex::A);
    case 0x60:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::B);
    case 0x61:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::C);
    case 0x62:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::D);
    case 0x63:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::E);
    case 0x64:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::H);
    case 0x65:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::L);
    case 0x66:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x67:
      return Instruction(Opcode::LD, RegisterByteIndex::H,
                         RegisterByteIndex::A);
    case 0x68:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::B);
    case 0x69:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::C);
    case 0x6A:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::D);
    case 0x6B:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::E);
    case 0x6C:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::H);
    case 0x6D:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::L);
    case 0x6E:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x6F:
      return Instruction(Opcode::LD, RegisterByteIndex::L,
                         RegisterByteIndex::A);
    case 0x70:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::B);
    case 0x71:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::C);
    case 0x72:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::D);
    case 0x73:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::E);
    case 0x74:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::H);
    case 0x75:
      return Instruction(Opcode::LD, AddressIndex(RegisterBytePairIndex::HL),
                         RegisterByteIndex::L);
    case 0x76:
      return Instruction(Opcode::HALT);
    case 0x78:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::B);
    case 0x79:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::C);
    case 0x7A:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::D);
    case 0x7B:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::E);
    case 0x7C:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::H);
    case 0x7D:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::L);
    case 0x7E:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x7F:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         RegisterByteIndex::A);
    case 0x80:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::B);
    case 0x81:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::C);
    case 0x82:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::D);
    case 0x83:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::E);
    case 0x84:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::H);
    case 0x85:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::L);
    case 0x86:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x87:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         RegisterByteIndex::A);
    case 0x88:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::B);
    case 0x89:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::C);
    case 0x8A:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::D);
    case 0x8B:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::E);
    case 0x8C:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::H);
    case 0x8D:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::L);
    case 0x8E:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x8F:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         RegisterByteIndex::A);
    case 0x90:
      return Instruction(Opcode::SUB, RegisterByteIndex::B);
    case 0x91:
      return Instruction(Opcode::SUB, RegisterByteIndex::C);
    case 0x92:
      return Instruction(Opcode::SUB, RegisterByteIndex::D);
    case 0x93:
      return Instruction(Opcode::SUB, RegisterByteIndex::E);
    case 0x94:
      return Instruction(Opcode::SUB, RegisterByteIndex::H);
    case 0x95:
      return Instruction(Opcode::SUB, RegisterByteIndex::L);
    case 0x96:
      return Instruction(Opcode::SUB, AddressIndex(RegisterBytePairIndex::HL));
    case 0x97:
      return Instruction(Opcode::SUB, RegisterByteIndex::A);
    case 0x98:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::B);
    case 0x99:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::C);
    case 0x9A:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::D);
    case 0x9B:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::E);
    case 0x9C:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::H);
    case 0x9D:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::L);
    case 0x9E:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         AddressIndex(RegisterBytePairIndex::HL));
    case 0x9F:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         RegisterByteIndex::A);
    case 0xA0:
      return Instruction(Opcode::AND, RegisterByteIndex::B);
    case 0xA1:
      return Instruction(Opcode::AND, RegisterByteIndex::C);
    case 0xA2:
      return Instruction(Opcode::AND, RegisterByteIndex::D);
    case 0xA3:
      return Instruction(Opcode::AND, RegisterByteIndex::E);
    case 0xA4:
      return Instruction(Opcode::AND, RegisterByteIndex::H);
    case 0xA5:
      return Instruction(Opcode::AND, RegisterByteIndex::L);
    case 0xA6:
      return Instruction(Opcode::AND, AddressIndex(RegisterBytePairIndex::HL));
    case 0xA7:
      return Instruction(Opcode::AND, RegisterByteIndex::A);
    case 0xA8:
      return Instruction(Opcode::XOR, RegisterByteIndex::B);
    case 0xA9:
      return Instruction(Opcode::XOR, RegisterByteIndex::C);
    case 0xAA:
      return Instruction(Opcode::XOR, RegisterByteIndex::D);
    case 0xAB:
      return Instruction(Opcode::XOR, RegisterByteIndex::E);
    case 0xAC:
      return Instruction(Opcode::XOR, RegisterByteIndex::H);
    case 0xAD:
      return Instruction(Opcode::XOR, RegisterByteIndex::L);
    case 0xAE:
      return Instruction(Opcode::XOR, AddressIndex(RegisterBytePairIndex::HL));
    case 0xAF:
      return Instruction(Opcode::XOR, RegisterByteIndex::A);
    case 0xB0:
      return Instruction(Opcode::OR, RegisterByteIndex::B);
    case 0xB1:
      return Instruction(Opcode::OR, RegisterByteIndex::C);
    case 0xB2:
      return Instruction(Opcode::OR, RegisterByteIndex::D);
    case 0xB3:
      return Instruction(Opcode::OR, RegisterByteIndex::E);
    case 0xB4:
      return Instruction(Opcode::OR, RegisterByteIndex::H);
    case 0xB5:
      return Instruction(Opcode::OR, RegisterByteIndex::L);
    case 0xB6:
      return Instruction(Opcode::OR, AddressIndex(RegisterBytePairIndex::HL));
    case 0xB7:
      return Instruction(Opcode::OR, RegisterByteIndex::A);
    case 0xB8:
      return Instruction(Opcode::CP, RegisterByteIndex::B);
    case 0xB9:
      return Instruction(Opcode::CP, RegisterByteIndex::C);
    case 0xBA:
      return Instruction(Opcode::CP, RegisterByteIndex::D);
    case 0xBB:
      return Instruction(Opcode::CP, RegisterByteIndex::E);
    case 0xBC:
      return Instruction(Opcode::CP, RegisterByteIndex::H);
    case 0xBD:
      return Instruction(Opcode::CP, RegisterByteIndex::L);
    case 0xBE:
      return Instruction(Opcode::CP, AddressIndex(RegisterBytePairIndex::HL));
    case 0xBF:
      return Instruction(Opcode::CP, RegisterByteIndex::A);
    case 0xC0:
      return Instruction(Opcode::RET, FlagIndex::NZ);
    case 0xC1:
      return Instruction(Opcode::POP, RegisterBytePairIndex::BC);
    case 0xC2:
      return Instruction(Opcode::JP, FlagIndex::NZ,
                         Address(immediate_byte_pair.value()));
    case 0xC3:
      return Instruction(Opcode::JP, Address(immediate_byte_pair.value()));
    case 0xC4:
      return Instruction(Opcode::CALL, FlagIndex::NZ,
                         Address(immediate_byte_pair.value()));
    case 0xC5:
      return Instruction(Opcode::PUSH, RegisterBytePairIndex::BC);
    case 0xC6:
      return Instruction(Opcode::ADD, RegisterByteIndex::A,
                         ImmediateByte(instruction_bytes[1]));
    case 0xC7:
      return Instruction(Opcode::RST, Address(0x00));
    case 0xC8:
      return Instruction(Opcode::RET, FlagIndex::Z);
    case 0xC9:
      return Instruction(Opcode::RET);
    case 0xCA:
      return Instruction(Opcode::JP, FlagIndex::Z,
                         Address(immediate_byte_pair.value()));
    case 0xCC:
      return Instruction(Opcode::CALL, FlagIndex::Z,
                         Address(immediate_byte_pair.value()));
    case 0xCD:
      return Instruction(Opcode::CALL, Address(immediate_byte_pair.value()));
    case 0xCE:
      return Instruction(Opcode::ADC, RegisterByteIndex::A,
                         ImmediateByte(instruction_bytes[1]));
    case 0xCF:
      return Instruction(Opcode::RST, Address(0x08));
    case 0xD0:
      return Instruction(Opcode::RET, FlagIndex::NC);
    case 0xD1:
      return Instruction(Opcode::POP, RegisterBytePairIndex::DE);
    case 0xD2:
      return Instruction(Opcode::JP, FlagIndex::NC,
                         Address(immediate_byte_pair.value()));
    case 0xD4:
      return Instruction(Opcode::CALL, FlagIndex::NC,
                         Address(immediate_byte_pair.value()));
    case 0xD5:
      return Instruction(Opcode::PUSH, RegisterBytePairIndex::DE);
    case 0xD6:
      return Instruction(Opcode::SUB, ImmediateByte(instruction_bytes[1]));
    case 0xD7:
      return Instruction(Opcode::RST, Address(0x10));
    case 0xD8:
      return Instruction(Opcode::RET, FlagIndex::C);
    case 0xD9:
      return Instruction(Opcode::RETI);
    case 0xDA:
      return Instruction(Opcode::JP, FlagIndex::C,
                         Address(immediate_byte_pair.value()));
    case 0xDC:
      return Instruction(Opcode::CALL, FlagIndex::C,
                         Address(immediate_byte_pair.value()));
    case 0xDE:
      return Instruction(Opcode::SBC, RegisterByteIndex::A,
                         ImmediateByte(instruction_bytes[1]));
    case 0xDF:
      return Instruction(Opcode::RST, Address(0x18));
    case 0xE0:
      return Instruction(Opcode::LDH, Address(instruction_bytes[1]),
                         RegisterByteIndex::A);
    case 0xE1:
      return Instruction(Opcode::POP, RegisterBytePairIndex::HL);
    case 0xE2:
      return Instruction(Opcode::LDH, RegisterByteIndex::C,
                         RegisterByteIndex::A);
    case 0xE5:
      return Instruction(Opcode::PUSH, RegisterBytePairIndex::HL);
    case 0xE6:
      return Instruction(Opcode::AND, ImmediateByte(instruction_bytes[1]));
    case 0xE7:
      return Instruction(Opcode::RST, Address(0x20));
    case 0xE8:
      return Instruction(Opcode::ADD, RegisterBytePairIndex::SP,
                         ImmediateByte(instruction_bytes[1]));
    case 0xE9:
      return Instruction(Opcode::JP, AddressIndex(RegisterBytePairIndex::HL));
    case 0xEA:
      return Instruction(Opcode::LD, Address(immediate_byte_pair.value()),
                         RegisterByteIndex::A);
    case 0xEE:
      return Instruction(Opcode::XOR, ImmediateByte(instruction_bytes[1]));
    case 0xEF:
      return Instruction(Opcode::RST, Address(0x28));
    case 0xF0:
      return Instruction(Opcode::LDH, RegisterByteIndex::A,
                         Address(instruction_bytes[1]));
    case 0xF1:
      return Instruction(Opcode::POP, RegisterBytePairIndex::AF);
    case 0xF2:
      return Instruction(Opcode::LDH, RegisterByteIndex::A,
                         RegisterByteIndex::C);
    case 0xF3:
      return Instruction(Opcode::DI);
    case 0xF5:
      return Instruction(Opcode::PUSH, RegisterBytePairIndex::AF);
    case 0xF6:
      return Instruction(Opcode::OR, ImmediateByte(instruction_bytes[1]));
    case 0xF7:
      return Instruction(Opcode::RST, Address(0x30));
    case 0xF8:
      return Instruction(Opcode::LDHL, RegisterBytePairIndex::SP,
                         ImmediateByte(instruction_bytes[1]));
    case 0xF9:
      return Instruction(Opcode::LD, RegisterBytePairIndex::SP,
                         RegisterBytePairIndex::HL);
    case 0xFA:
      return Instruction(Opcode::LD, RegisterByteIndex::A,
                         Address(immediate_byte_pair.value()));
    case 0xFB:
      return Instruction(Opcode::EI);
    case 0xFE:
      return Instruction(Opcode::CP, ImmediateByte(instruction_bytes[1]));
    case 0xFF:
      return Instruction(Opcode::RST, Address(0x38));
  }
  FATAL("Could not decode bytes %x %x %x into an instruction",
        instruction_bytes[0], instruction_bytes[1], instruction_bytes[2]);
}

Instruction::Instruction(Opcode op) : op_(op) {}
Instruction::Instruction(Opcode op, InstructionOperand operand1)
    : op_(op), operand1_(operand1) {}
Instruction::Instruction(Opcode op, InstructionOperand operand1,
                         InstructionOperand operand2)
    : op_(op), operand1_(operand1), operand2_(operand2) {}

}  // namespace gamebun
