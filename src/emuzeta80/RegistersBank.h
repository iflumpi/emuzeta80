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
 * @file RegisterBank.h
 * @ingroup emuzeta80
 * @author Lumpi (iflumpi@gmail.com)
 * @version 0.1
 *
 * @brief  Definition of 16-bit bank registers for Z80
 *
 */

#pragma once

#include <cstdint>
#include "Register.h"

namespace emuzeta80
{

enum Flag
{
    FLAG_S = 0x80, //< Sign flag: Set if the 2-complement value is negative (copy of MSB)
    FLAG_Z = 0x40, //< Zero flag: Set if the value is zero
    FLAG_5 = 0x20, //< 5-flag (reserved)
    FLAG_H = 0x10, //< Half carry flag: Carry from bit 3 to bit 4
    FLAG_3 = 0x08, //< 3-flag (reserved)
    FLAG_P = 0x04, //< Parity/Overflow flag: Parity set if even number of bits set /  Overflow set if the 2-complement result does not fit in the register
    FLAG_N = 0x02, //< DAA flag: Set if the last operation was a subtraction
    FLAG_C = 0x01  //< Carry flag: Set if the result did not fit in the register
};

struct RegistersBank
{
    Register bc, de, hl, af;

    bool getFlag(Flag flag)
    {
        return af.bytes.L & flag;
    }

    void setFlag(Flag flag, bool value)
    {
        if(value)
            af.bytes.L = af.bytes.L | flag;
        else
            af.bytes.L = af.bytes.L & ~flag;
    }
};

} // namespace emuzeta80
