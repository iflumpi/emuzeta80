/*
 * emuzeta80 (emulator for z80)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

/**
 * @file ALU.h
 * @ingroup emuzeta80
 * @author Lumpi (iflumpi@gmail.com)
 * @version 0.1
 *
 * @brief ALU class to implement arithmetic and logic operations
 *
 */

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "RegistersBank.h"
#include <cstdint>

//-------------------------------------------------------------------------
// Class definition
//-------------------------------------------------------------------------

namespace emuzeta80 {
class ALU {
public:
  ALU(RegistersBank *mainBank);

  // ---------------------
  // Arithmetic operations
  // ---------------------
  uint16_t inc8(Register *reg16, bool high);
  uint16_t dec8(Register *reg8, bool high);
  uint16_t inc16(Register *reg16);
  uint16_t add16(Register *reg16A, Register *reg16B);
  uint16_t dec16(Register *reg16);
  uint16_t add8(Register *reg8A, bool highA, Register *reg8B, bool highB,
                bool carry = false);
  uint16_t add8(Register *reg8A, bool highA, char value, bool carry = false);
  uint16_t sub8(Register *reg8A, bool highA, Register *reg8B, bool highB,
                bool carry = false);
  uint16_t sub8(Register *reg8A, bool highA, char value, bool carry = false);

  // ----------------
  // Logic operations
  // ----------------
  uint16_t and8(Register *reg8A, bool highA, Register *reg8B, bool highB);
  uint16_t and8(Register *reg8A, bool highA, char value);
  uint16_t or8(Register *reg8A, bool highA, Register *reg8B, bool highB);
  uint16_t or8(Register *reg8A, bool highA, char value);
  uint16_t xor8(Register *reg8A, bool highA, Register *reg8B, bool highB);
  uint16_t xor8(Register *reg8A, bool highA, char value);
  uint16_t cp8(Register *reg8A, bool highA, Register *reg8B, bool highB);
  uint16_t cp8(Register *reg8A, bool highA, char value);

protected:
  RegistersBank *mainBank;
};

} // namespace emuzeta80
