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
 * @file CPU.cpp
 * @ingroup emuzeta80
 * @author Lumpi (iflumpi@gmail.com)
 * @version 0.1
 *
 * @brief CPU class for the Z80 emulator
 *
 */

#pragma once

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include <cstdint>
#include "ALU.h"
#include "RAM.h"
#include "RegistersBank.h"

//-------------------------------------------------------------------------
// Class definition
//-------------------------------------------------------------------------

namespace emuzeta80
{

class CPU
{
public:
    CPU(uint16_t ramSize);

    uint16_t execute();
    uint16_t getPC();
    uint16_t getAF();
    uint16_t getBC();
    uint16_t getDE();
    uint16_t getHL();
    uint16_t getClockCycles();
    void incPC();
    void setPC(uint16_t value);
    void writeMemory(uint8_t value);

protected:
    uint16_t jp(bool condition);
    uint16_t call(bool condition);
    uint16_t ret(bool condition);
    uint16_t rst(uint16_t address);
    uint16_t ld8mem(Register* reg16, bool high);
    uint16_t inc8mem(uint16_t address);
    uint16_t dec8mem(uint16_t address);

public:
    RAM* memory;
    ALU* alu;
    RegistersBank mainBank;
    RegistersBank alternateBank;
    Register pc; //< Program Counter
    Register sp; //< Stack Pointer
    Register iX; //< Index X;
    Register iY; //< Index Y;
    uint8_t i;   //< Interruption Vector
    uint8_t r;   //< Memory Refresh
    uint16_t clockCycles = 0;
};

} // namespace emuzeta80
