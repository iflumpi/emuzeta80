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
  uint16_t inc8(uint8_t* value);
  uint16_t dec8(uint8_t* value);
  uint16_t dec8mem(uint16_t address);
  uint16_t add16(uint16_t* target, uint16_t source, bool carry = false);
  uint16_t add8(uint8_t* target, uint8_t source, bool carry = false);
  uint16_t sub8(uint8_t* target, uint8_t source, bool carry = false);
  uint16_t sub16(uint16_t* target, uint16_t source, bool carry = false);

  // ----------------
  // Logic operations
  // ----------------
  uint16_t and8(uint8_t* target, uint8_t value);
  uint16_t or8(uint8_t* target, uint8_t value);
  uint16_t xor8(uint8_t* target, uint8_t value);
  uint16_t cp8(uint8_t target, uint8_t value);

  // -----------------
  // Bits manipulation
  // -----------------  
  uint16_t rlc(uint8_t* value);
  uint16_t rrc(uint8_t* value);
  uint16_t rl(uint8_t* value);
  uint16_t rr(uint8_t* value);
  uint16_t sla(uint8_t* value);
  uint16_t sra(uint8_t* value);
  uint16_t sll(uint8_t* value);
  uint16_t srl(uint8_t* value);
  uint16_t bit(uint8_t* value, uint8_t bitPosition);
  uint16_t res(uint8_t* value, uint8_t bitPosition);
  uint16_t set(uint8_t* value, uint8_t bitPosition);
  uint16_t neg(uint8_t* value);

protected:
  RegistersBank *mainBank;
};

} // namespace emuzeta80
