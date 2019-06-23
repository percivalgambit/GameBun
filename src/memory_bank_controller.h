#ifndef MEMORY_BANK_CONTROLLER_H_
#define MEMORY_BANK_CONTROLLER_H_

#include "address.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <variant>

namespace gamebun {

// TODO: Add MMM01
// TODO: Add Pocket Camera
// TODO: Add Bandai TAMA5
// TODO: Add Hudson HuC-3
// TODO: Add Hudson HuC-1
enum class MemoryBankControllerType {
  kNone,
  kController1,
  kController2,
  kController3,
  kController5,
};

class MemoryBankController {
 public:
  static std::unique_ptr<MemoryBankController> SelectController(
      MemoryBankControllerType controller_type);

  virtual ~MemoryBankController() {}

  virtual void Write(Address address, uint8_t value) = 0;

  virtual size_t GetSelectedRomBank() const = 0;
  virtual size_t GetSelectedRamBank() const = 0;
  virtual bool RamEnabled() const = 0;
};

class NoController : public MemoryBankController {
 public:
  void Write(Address address, uint8_t value) override;

  size_t GetSelectedRomBank() const override;
  size_t GetSelectedRamBank() const override;
  bool RamEnabled() const override;
};

class Controller1 : public MemoryBankController {
 public:
  Controller1();

  void Write(Address address, uint8_t value) override;

  size_t GetSelectedRomBank() const override;
  size_t GetSelectedRamBank() const override;
  bool RamEnabled() const override;

 private:
  enum class MemoryMode {
    k16MRom8KRam,
    k4MRom32KRam,
  };

  MemoryMode memory_mode_;
  size_t selected_ram_bank_;
  size_t selected_rom_bank_low_;
  size_t selected_rom_bank_high_;
  bool ram_enabled_;
};

class Controller2 : public MemoryBankController {
 public:
  Controller2();

  void Write(Address address, uint8_t value) override;

  size_t GetSelectedRomBank() const override;
  size_t GetSelectedRamBank() const override;
  bool RamEnabled() const override;

 private:
  size_t selected_rom_bank_;
  bool ram_enabled_;
};

class Controller3 : public MemoryBankController {
 public:
  void Write(Address address, uint8_t value) override;

  size_t GetSelectedRomBank() const override;
  size_t GetSelectedRamBank() const override;
  bool RamEnabled() const override;
};

class Controller5 : public MemoryBankController {
 public:
  void Write(Address address, uint8_t value) override;

  size_t GetSelectedRomBank() const override;
  size_t GetSelectedRamBank() const override;
  bool RamEnabled() const override;
};

}  // namespace gamebun

#endif  // MEMORY_BANK_CONTROLLER_H_
