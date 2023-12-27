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

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "CPU.h"
#include <cstdio>

//-------------------------------------------------------------------------
// Class implementation
//-------------------------------------------------------------------------

namespace emuzeta80
{

/**
 * @brief Construct a new CPU instance
 *
 * Create a RAM memory of 64 kb
 * Create ALU for arithmetical and logic operations
 * Initialize values of PC, SP, iX and iY registers to 0
 */
CPU::CPU(uint16_t ramSize)
{
    memory = new RAM(ramSize); // 64 kb
    alu = new ALU(&mainBank);

    pc.value = 0;
    sp.value = 0;
    iX.value = 0;
    iY.value = 0;
    i = 0;
    r = 0;
}

/**
 * @brief Get current value of PC register
 *
 * @return value of PC register
 */
uint16_t CPU::getpc()
{
    return pc.value;
}

/**
 * @brief Get current value of SP register
 *
 * @return value of SP register
 */
uint16_t CPU::getsp()
{
    return pc.value;
}

/**
 * @brief Get current value of AF register
 *
 * @alt if true return AF' (alternate bank) else AF (main bank)
 * @return value of AF register
 */
uint16_t CPU::getaf(bool alt)
{
    return alt ? alternateBank.af.value : mainBank.af.value;
}

/**
 * @brief Get current value of BC register
 *
 * @alt if true return BC' (alternate bank) else BC (main bank)
 * @return value of BC register
 */
uint16_t CPU::getbc(bool alt)
{
    return alt ? alternateBank.bc.value : mainBank.bc.value;
}

/**
 * @brief Get current value of DE register
 *
 * @alt if true return DE' (alternate bank) else DE (main bank)
 * @return value of DE register
 */
uint16_t CPU::getde(bool alt)
{
    return alt ? alternateBank.de.value : mainBank.de.value;
}

/**
 * @brief Get current value of HL register
 *
 * @alt if true return HL' (alternate bank) else HL (main bank)
 * @return value of HL register
 */
uint16_t CPU::gethl(bool alt)
{
    return alt ? alternateBank.hl.value : mainBank.hl.value;
}

/**
 * @brief Get total of consumed clock cycles
 *
 * @return number of consumed clock cycles
 */
uint16_t CPU::getClockCycles()
{
    return clockCycles;
}

/**
 * @brief Increase value of PC register by one
 *
 */
void CPU::incpc()
{
    pc.value++;
}

/**
 * @brief Set value of Program Counter (PC) register to a specific value
 *
 * @param value Value to set the PC register
 */
void CPU::setpc(uint16_t value)
{
    pc.value = value;
}

/**
 * @brief Read value from a position of memory
 *
 * @param address memory address to read value (if not set value is written in memory pointed by PC register)
 * @return value retrieved from memory address
 */
uint8_t CPU::read(uint16_t address)
{
    if(address < 0)
        address = pc.value;

    return memory->peek(address);
}

/**
 * @brief Write value into a position of memory
 *
 * @param value 8-bit value to be written in memory
 * @param address memory address to write value (if not set value is written in memory pointed by PC register)
 */
void CPU::write(uint8_t value, uint16_t address)
{
    if(address < 0)
        address = pc.value;

    memory->poke(address, value);
}

/**
 * @brief Conditional jump based on the specified condition
 *
 * @param condition The boolean condition that determines whether to perform the jump
 * @return number of cycles of the operation
 */
uint16_t CPU::jp(bool condition)
{
    if(condition)
    {
        auto value = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        pc.value = value;
    }
    else
        pc.value += 2;

    return 10;
}

/**
 * @brief Conditional subroutine call base on the specified condiction
 *
 * @param condition The boolean condition that determines whether to perform the call operation
 * @return number of cycles of the operation
 */
uint16_t CPU::call(bool condition)
{
    if(condition)
    {
        auto value = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        memory->poke(--sp.value, pc.bytes.H);
        memory->poke(--sp.value, pc.bytes.L);
        pc.value = value;

        return 17;
    }

    return 10;
}

/**
 * @brief Conditional return based on the specified condition
 *
 * @param condition The boolean condition that determines whether to perform the ret operation
 * @return number of cycles of the operation
 */
uint16_t CPU::ret(bool condition)
{
    if(condition)
    {
        pc.value = memory->peek(sp.value++) + (memory->peek(sp.value++) << 8);
        return 11;
    }

    return 5;
}

/**
 * @brief Subroutine call to the specified address
 *
 * @param address target address
 * @return number of cycles of the operation
 */
uint16_t CPU::rst(uint16_t address)
{
    pc.value++;
    memory->poke(--sp.value, pc.bytes.H);
    memory->poke(--sp.value, pc.bytes.L);
    pc.value = address;

    return 11;
}

/**
 * @brief Increases value of a byte in memory by one
 *
 * @param address memory address with value to be increased
 * @return number of cycles of the operation
 */
uint16_t CPU::inc8mem(uint16_t address)
{
    uint8_t value = memory->peek(address);
    uint8_t updatedValue = value + 1;
    memory->poke(address, updatedValue);

    mainBank.setFlag(Flag::FLAG_N, false);
    mainBank.setFlag(Flag::FLAG_P, value == 0x7F);
    mainBank.setFlag(Flag::FLAG_H, value == 0x0F);
    mainBank.setFlag(Flag::FLAG_Z, updatedValue == 0);
    mainBank.setFlag(Flag::FLAG_S, updatedValue == 0x80);

    return 11;
}

/**
 * @brief Decreases value of a byte in memory by one
 *
 * @param address memory address with value to be decreased
 * @return uint16_t number of cycles of the operation
 */
uint16_t CPU::dec8mem(uint16_t address)
{
    uint8_t value = memory->peek(address);
    uint8_t updatedValue = value - 1;
    memory->poke(address, updatedValue);

    mainBank.setFlag(Flag::FLAG_N, true);
    mainBank.setFlag(Flag::FLAG_P, value == 0x80); // -128 = 0x80 in 2a complement
    mainBank.setFlag(Flag::FLAG_H, updatedValue == 0x0F);
    mainBank.setFlag(Flag::FLAG_Z, updatedValue == 0);
    mainBank.setFlag(Flag::FLAG_S, updatedValue == 0x7F);

    return 11;
}

/**
 * @brief Load contents of a memory cell to a 16-bit register
 *
 * @param reg16 target register (bc, de, hl, af)
 * @param high flag to specify if select the higher or lower byte of the target register
 * @return uint16_t number of cycles of the operation
 */
uint16_t CPU::ld8mem(Register* reg16, bool high)
{
    if(high)
        reg16->bytes.H = memory->peek(pc.value++);
    else
        reg16->bytes.L = memory->peek(pc.value++);

    return 7;
}

/**
 * @brief Execute instruction pointed by PC register
 *
 * This method processes the following steps.
 *  - Get byte pointed by PC register
 *  - Increase value of PC register by 1
 *  - Execute instruction according to the value of retrieved byte
 * 		- some instructions requires more than one byte
 * 		- in these cases, the bytes are retrieved and the value of PC register is increased according to the number of bytes
 * 	- Update flags if required
 *  - Update total number of cycle clocks
 *
 * @return uint16_t
 */
uint16_t CPU::execute()
{
    auto opcode = memory->peek(pc.value++);

    switch(opcode)
    {
    case 0x0:
    {
        // 0: NOP

        clockCycles += 4;
        break;
    }

    case 0x01:
    {
        // 1: LD BC, **
        // Load content of memory to the BC register
        // Flags affected: None

        mainBank.bc.bytes.L = memory->peek(pc.value++);
        mainBank.bc.bytes.H = memory->peek(pc.value++);
        clockCycles += 10;
        break;
    }

    case 0x02:
    {
        // 2: LD (BC), A
        // Store A register into the position of memory located by BC register
        // Flags affected: None

        memory->poke(mainBank.bc.value, mainBank.af.bytes.H);
        clockCycles += 7;
        break;
    }

    case 0x03:
    {
        // 3: INC BC
        // Increase BC register by 1
        // Flags affected: None

        mainBank.bc.value += 1;
        clockCycles += 6;
        break;
    }

    case 0x04:
    {
        // 4: INC B
        // Increases value of B by one
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.bc), true);
        break;
    }

    case 0x05:
    {
        // 5: DEC B
        // Decrements value of B by one
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.bc), true);
        break;
    }

    case 0x06:
    {
        // 6: LD B, *
        // Loads contents of memory (*) to B
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.bc), true);
        break;
    }

    case 0x07:
    {
        // 7: RLCA
        // The contents of a are rotated left one bit position
        // The value of the bit 7 are copied to FLAG_C and bit 0
        // Flags affected: C, N, H

        auto bit7 = (mainBank.af.bytes.H & 0x80) == 1;
        mainBank.af.bytes.H = mainBank.af.bytes.H << 1;
        mainBank.af.bytes.H = bit7 ? mainBank.af.bytes.H | 0x01 : mainBank.af.bytes.H & 0xFE;

        mainBank.setFlag(Flag::FLAG_C, bit7);
        mainBank.setFlag(Flag::FLAG_N, 0);
        mainBank.setFlag(Flag::FLAG_H, 0);

        clockCycles += 4;
        break;
    }

    case 0x08:
    {
        // 8: EX AF, AF'
        // Exchanges the contents of AF and AF'

        uint16_t af = mainBank.af.value;
        mainBank.af.value = alternateBank.af.value;
        alternateBank.af.value = mainBank.af.value;

        clockCycles += 4;
        break;
    }

    case 0x09:
    {
        // 9: ADD HL, BC
        // Adds the value of BC to HL
        // Flags affected: C, N, H

        clockCycles += alu->add16(&(mainBank.hl), &(mainBank.bc));
        break;
    }

    case 0x0A:
    {
        // 10: LD A, (BC)
        // Loads the content of memory pointed by BC to A
        // Flags affected: None

        mainBank.af.bytes.H = memory->peek(mainBank.bc.value);

        clockCycles += 7;
        break;
    }

    case 0x0B:
    {
        // 11: DEC BC
        // Decreases the value of BC by 1
        // Flags affected: None

        mainBank.bc.value -= 1;

        clockCycles += 6;
        break;
    }

    case 0x0C:
    {
        // 12: INC C
        // Increase the value of C by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.bc), false);
        break;
    }

    case 0x0D:
    {
        // 13: DEC C
        // Decrease the value of C by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.bc), false);
        break;
    }

    case 0x0E:
    {
        // 14: LD C, *
        // Loads contents of memory (*) to B
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.bc), false);
        break;
    }

    case 0x0F:
    {
        // 15: RRCA
        // The contents of a are rotated right one bit position
        // The value of the bit 0 are copied to FLAG_C and bit 7
        // Flags affected: C, N, H

        auto bit0 = (mainBank.af.bytes.H & 0x01) == 1;
        mainBank.af.bytes.H = mainBank.af.bytes.H >> 1;
        mainBank.af.bytes.H = bit0 ? mainBank.af.bytes.H | 0x80 : mainBank.af.bytes.H & 0xEF;

        mainBank.setFlag(Flag::FLAG_C, bit0);
        mainBank.setFlag(Flag::FLAG_N, 0);
        mainBank.setFlag(Flag::FLAG_H, 0);

        clockCycles += 4;
        break;
    }

    case 0x10:
    {
        // 16: DJNZ *
        // The B register is decremented by one
        // If the result is not zero, PC is incremented by *
        // Flags affected: None

        if(--mainBank.bc.bytes.H != 0)
        {
            pc.value += (char)memory->peek(pc.value);
            clockCycles += 5;
        }

        clockCycles += 8;
        break;
    }

    case 0x11:
    {
        // 17: LD DE, **
        // Load content of memory to the DE register
        // Flags affected: None

        mainBank.de.bytes.L = memory->peek(pc.value++);
        mainBank.de.bytes.H = memory->peek(pc.value++);
        clockCycles += 10;
        break;
    }

    case 0x12:
    {
        // 18: LD (DE), A
        // Store A register into the position of memory located by DE
        // Flags affected: None

        memory->poke(mainBank.de.value, mainBank.af.bytes.H);
        clockCycles += 7;
        break;
    }

    case 0x13:
    {
        // 19: INC DE
        // Increase DE register by 1
        // Flags affected: None

        mainBank.de.value += 1;
        clockCycles += 6;
        break;
    }

    case 0x14:
    {
        // 20: INC D
        // Increases value of D by one
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.de), true);
        break;
    }

    case 0x15:
    {
        // 21: DEC D
        // Decrements value of D by one
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.de), true);
        break;
    }

    case 0x16:
    {
        // 22: LD D, *
        // Loads contents of memory (*) to D
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.de), true);
        break;
    }

    case 0x17:
    {
        // 23: RLA
        // The contents of a are rotated left one bit position
        // The value of the bit 7 is copied to FLAG_C
        // The previous value of FLAG_C is copied to bit 0
        // Flags affected: C, N, H

        auto bit7 = (mainBank.af.bytes.H & 0x80) == 1;
        auto flagC = mainBank.getFlag(Flag::FLAG_C) == 1;
        mainBank.af.bytes.H = mainBank.af.bytes.H << 1;
        mainBank.af.bytes.H = flagC ? mainBank.af.bytes.H | 0x01 : mainBank.af.bytes.H & 0xFE;

        mainBank.setFlag(Flag::FLAG_C, bit7);
        mainBank.setFlag(Flag::FLAG_N, 0);
        mainBank.setFlag(Flag::FLAG_H, 0);

        clockCycles += 4;
        break;
    }

    case 0x18:
    {
        // 24: JR *
        // PC is increase by * (signed value)
        // Flags affected: None

        pc.value += (char)memory->peek(pc.value);

        clockCycles += 12;
        break;
    }

    case 0x19:
    {
        // 25: ADD HL, DE
        // Adds the value of DE to HL
        // Flags affected: C, N, H

        clockCycles += alu->add16(&(mainBank.hl), &(mainBank.de));
        break;
    }

    case 0x1A:
    {
        // 26: LD A, (DE)
        // Loads the content of memory pointed by DE to A
        // Flags affected: None

        mainBank.af.bytes.H = memory->peek(mainBank.de.value);

        clockCycles += 7;
        break;
    }

    case 0x1B:
    {
        // 27: DEC DE
        // Decreases the value of DE by 1
        // Flags affected: None

        mainBank.de.value -= 1;

        clockCycles += 6;
        break;
    }

    case 0x1C:
    {
        // 28: INC E
        // Increase the value of E by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.de), false);
        break;
    }

    case 0x1D:
    {
        // 29: DEC E
        // Decrease the value of E by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.de), false);
        break;
    }

    case 0x1E:
    {
        // 30: LD E, *
        // Loads contents of memory (*) to E
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.de), false);
        break;
    }

    case 0x1F:
    {
        // 31: RRCA
        // The contents of a are rotated right one bit position
        // The value of the bit 0 are copied to FLAG_C
        // The previous value of FLAG_C is copied to bit 7
        // Flags affected: C, N, H

        auto bit0 = (mainBank.af.bytes.H & 0x01) == 1;
        auto flagc = mainBank.getFlag(Flag::FLAG_C) == 1;
        mainBank.af.bytes.H = mainBank.af.bytes.H >> 1;
        mainBank.af.bytes.H = flagc ? mainBank.af.bytes.H | 0x80 : mainBank.af.bytes.H & 0xEF;

        mainBank.setFlag(Flag::FLAG_C, bit0);
        mainBank.setFlag(Flag::FLAG_N, 0);
        mainBank.setFlag(Flag::FLAG_H, 0);

        clockCycles += 4;
        break;
    }

    case 0x20:
    {
        // 32: JR NZ, *
        // Increase PC by * if FLAG_Z is 0
        // Flags affected: None

        if(mainBank.getFlag(Flag::FLAG_Z) == 0)
        {
            pc.value += memory->peek(pc.value);
            clockCycles += 5;
        }

        clockCycles += 7;
        break;
    }

    case 0x21:
    {
        // 33: LD HL, **
        // Load content of memory to the DE register
        // Flags affected: None

        mainBank.hl.bytes.L = memory->peek(pc.value++);
        mainBank.hl.bytes.H = memory->peek(pc.value++);
        clockCycles += 10;
        break;
    }

    case 0x22:
    {
        // 34: LD (**), HL
        // Load content of HL register into memory
        // Flags affected: None

        auto address = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        memory->poke(address, mainBank.hl.bytes.L);
        memory->poke(address + 1, mainBank.hl.bytes.H);
        clockCycles += 16;
        break;
    }

    case 0x23:
    {
        // 35: INC HL
        // Increase HL register by 1
        // Flags affected: None

        mainBank.hl.value += 1;
        clockCycles += 6;
        break;
    }

    case 0x24:
    {
        // 36: INC H
        // Increases value of D by one
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.hl), true);
        break;
    }

    case 0x25:
    {
        // 37: DEC H
        // Decrements value of H by one
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.hl), true);
        break;
    }

    case 0x26:
    {
        // 38: LD H, *
        // Loads contents of memory (*) to H
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.hl), true);
        break;
    }

    case 0x27:
    {
        // 39: DAA

        // TODO
        // Implements DAA instruction

        clockCycles += 4;
        break;
    }

    case 0x28:
    {
        // 40: JR Z, *
        // PC is increase by * (signed value) if FLAG_Z is set
        // Flags affected: None

        if(mainBank.getFlag(Flag::FLAG_Z) == 1)
        {
            pc.value += (char)memory->peek(pc.value);
            clockCycles += 5;
        }

        clockCycles += 7;
        break;
    }

    case 0x29:
    {
        // 41: ADD HL, HL
        // Adds the value of HL to HL
        // Flags affected: C, N, H

        clockCycles += alu->add16(&(mainBank.hl), &(mainBank.hl));
        break;
    }

    case 0x2A:
    {
        // 42: LD HL, (**)
        // Loads the content of memory pointed by ** to HL (H<-(**+1), L<-(**))
        // Flags affected: None

        auto address = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        mainBank.hl.bytes.L = memory->peek(address);
        mainBank.hl.bytes.H = memory->peek(address + 1);

        clockCycles += 16;
        break;
    }

    case 0x2B:
    {
        // 43: DEC HL
        // Decreases the value of HL by 1
        // Flags affected: None

        mainBank.hl.value -= 1;

        clockCycles += 6;
        break;
    }

    case 0x2C:
    {
        // 44: INC L
        // Increase the value of L by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.hl), false);
        break;
    }

    case 0x2D:
    {
        // 45: DEC L
        // Decrease the value of L by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.hl), false);
        break;
    }

    case 0x2E:
    {
        // 46: LD L, *
        // Loads contents of memory (*) to L
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.hl), false);
        break;
    }

    case 0x2F:
    {
        // 48: CPA
        // Inverts the value of register A
        // Flags affected: N, H

        // TODO
        // Implement CPA instruction

        mainBank.af.bytes.H = (char)~mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x30:
    {
        // 49: JR NC, *
        // Increase PC by * if FLAG_C is 0
        // Flags affected: None

        if(mainBank.getFlag(Flag::FLAG_C) == 0)
        {
            pc.value += memory->peek(pc.value);
            clockCycles += 5;
        }

        clockCycles += 7;
        break;
    }

    case 0x31:
    {
        // 50: LD SP, **
        // Load content of memory to the SP register
        // Flags affected: None

        sp.bytes.L = memory->peek(pc.value++);
        sp.bytes.H = memory->peek(pc.value++);

        clockCycles += 10;
        break;
    }

    case 0x32:
    {
        // 51: LD (**), A
        // Load content of A register into memory
        // Flags affected: None

        auto address = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        memory->poke(address, mainBank.af.bytes.H);

        clockCycles += 13;
        break;
    }

    case 0x33:
    {
        // 52: INC SP
        // Increase SP register by 1
        // Flags affected: None

        mainBank.hl.value += 1;

        clockCycles += 6;
        break;
    }

    case 0x34:
    {
        // 53: INC (HL)
        // Increase value of memory pointed by HL register by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += inc8mem(mainBank.hl.value);
        break;
    }

    case 0x35:
    {
        // 54: DEC (HL)
        // Decrease value of memory pointed by HL register by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += dec8mem(mainBank.hl.value);
        break;
    }

    case 0x36:
    {
        // 54: LD (HL), *
        // Load * into position of memory pointed by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, memory->peek(pc.value++));

        clockCycles += 10;
        break;
    }

    case 0x37:
    {
        // 55: SCF
        // Sets FLAG_C to true
        // Flags affected: C, N, H

        mainBank.setFlag(Flag::FLAG_C, true);
        mainBank.setFlag(Flag::FLAG_H, false);
        mainBank.setFlag(Flag::FLAG_N, false);

        clockCycles += 4;
        break;
    }

    case 0x38:
    {
        // 56: JR C, *
        // PC is increase by * (signed value) if FLAG_C is set
        // Flags affected: None

        if(mainBank.getFlag(Flag::FLAG_C) == 1)
        {
            pc.value += (char)memory->peek(pc.value);
            clockCycles += 5;
        }

        clockCycles += 7;
        break;
    }

    case 0x39:
    {
        // 57: ADD HL, SP
        // Adds the value of SP to HL
        // Flags affected: C, N, H

        clockCycles += alu->add16(&(mainBank.hl), &sp);
        break;
    }

    case 0x3A:
    {
        // 58: LD A, (**)
        // Loads the content of memory pointed by ** to A
        // Flags affected: None

        auto address = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        mainBank.af.bytes.H = memory->peek(address);

        clockCycles += 13;
        break;
    }

    case 0x3B:
    {
        // 59: DEC SP
        // Decreases the value of SP by 1
        // Flags affected: None

        sp.value -= 1;

        clockCycles += 6;
        break;
    }

    case 0x3C:
    {
        // 60: INC A
        // Increase the value of A by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->inc8(&(mainBank.af), true);
        break;
    }

    case 0x3D:
    {
        // 61: DEC
        // Decrease the value of A by 1
        // Flags affected: N, P, H, Z, S

        clockCycles += alu->dec8(&(mainBank.af), true);
        break;
    }

    case 0x3E:
    {
        // 62: LD A, *
        // Loads contents of memory (*) to A
        // Flags affected: None

        clockCycles += ld8mem(&(mainBank.af), true);
        break;
    }

    case 0x3F:
    {
        // 63: CCF
        // Inverts the value of FLAG_C
        // Flags affected: C, N, H

        mainBank.setFlag(Flag::FLAG_H, mainBank.getFlag(Flag::FLAG_C));
        mainBank.setFlag(Flag::FLAG_C, ~mainBank.getFlag(Flag::FLAG_C));
        mainBank.setFlag(Flag::FLAG_N, false);

        clockCycles += 4;
        break;
    }

    case 0x40:
    {
        // 64: LD B, B
        // Loads B into B
        // Flags affected: None

        // Nothing to do (B <- B)

        clockCycles += 4;
        break;
    }

    case 0x41:
    {
        // 65: LD B, C
        // Loads C into B
        // Flags affected: None

        mainBank.bc.bytes.H = mainBank.bc.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x42:
    {
        // 66: LD B, D
        // Loads D into B
        // Flags affected: None

        mainBank.bc.bytes.H = mainBank.de.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x43:
    {
        // 67: LD B, E
        // Loads E into B
        // Flags affected: None

        mainBank.bc.bytes.H = mainBank.de.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x44:
    {
        // 68: LD B, H
        // Loads H into B
        // Flags affected: None

        mainBank.bc.bytes.H = mainBank.hl.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x45:
    {
        // 69: LD B, L
        // Loads L into B
        // Flags affected: None

        mainBank.bc.bytes.H = mainBank.hl.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x46:
    {
        // 70: LD B, (HL)
        // Loads contents of memory pointed by HL into B
        // Flags affected: None

        mainBank.bc.bytes.H = memory->peek(mainBank.hl.value);

        clockCycles += 7;
        break;
    }

    case 0x47:
    {
        // 71: LD B, A
        // Loads A into B
        // Flags affected: None

        mainBank.bc.bytes.H = mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x48:
    {
        // 72: LD C, B
        // Loads B into C
        // Flags affected: None

        mainBank.bc.bytes.L = mainBank.bc.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x49:
    {
        // 73: LD C, C
        // Loads C into C
        // Flags affected: None

        // Nothing to do (C <- C)

        clockCycles += 4;
        break;
    }

    case 0x4A:
    {
        // 74: LD C, D
        // Loads D into C
        // Flags affected: None

        mainBank.bc.bytes.L = mainBank.de.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x4B:
    {
        // 75: LD C, E
        // Loads E into C
        // Flags affected: None

        mainBank.bc.bytes.L = mainBank.de.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x4C:
    {
        // 76: LD C, H
        // Loads H into C
        // Flags affected: None

        mainBank.bc.bytes.L = mainBank.hl.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x4D:
    {
        // 77: LD C, L
        // Loads L into C
        // Flags affected: None

        mainBank.bc.bytes.L = mainBank.hl.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x4E:
    {
        // 78: LD C, (HL)
        // Loads content of memory pointed by HL into C
        // Flags affected: None

        mainBank.bc.bytes.L = memory->peek(mainBank.hl.value);

        clockCycles += 4;
        break;
    }

    case 0x4F:
    {
        // 79: LD C, A
        // Loads A into C
        // Flags affected: None

        mainBank.bc.bytes.L = mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x50:
    {
        // 80: LD D, B
        // Loads B into D
        // Flags affected: None

        mainBank.de.bytes.H = mainBank.bc.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x51:
    {
        // 81: LD D, C
        // Loads C into D
        // Flags affected: None

        mainBank.de.bytes.H = mainBank.bc.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x52:
    {
        // 82: LD D, D
        // Loads D into D
        // Flags affected: None

        // Nothing to do (D <- D)

        clockCycles += 4;
        break;
    }

    case 0x53:
    {
        // 83: LD D, E
        // Loads E into D
        // Flags affected: None

        mainBank.de.bytes.H = mainBank.de.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x54:
    {
        // 84: LD D, H
        // Loads H into D
        // Flags affected: None

        mainBank.de.bytes.H = mainBank.hl.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x55:
    {
        // 85: LD D, L
        // Loads L into D
        // Flags affected: None

        mainBank.de.bytes.H = mainBank.hl.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x56:
    {
        // 86: LD D, (HL)
        // Loads contents of memory pointed by HL into D
        // Flags affected: None

        mainBank.de.bytes.H = memory->peek(mainBank.hl.value);

        clockCycles += 7;
        break;
    }

    case 0x57:
    {
        // 87: LD D, A
        // Loads A into D
        // Flags affected: None

        mainBank.de.bytes.H = mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x58:
    {
        // 88: LD E, B
        // Loads B into E
        // Flags affected: None

        mainBank.de.bytes.L = mainBank.bc.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x59:
    {
        // 89: LD E, C
        // Loads C into E
        // Flags affected: None

        mainBank.de.bytes.L = mainBank.bc.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x5A:
    {
        // 90: LD E, D
        // Loads D into E
        // Flags affected: None

        mainBank.de.bytes.L = mainBank.de.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x5B:
    {
        // 91: LD E, E
        // Loads E into E
        // Flags affected: None

        // Nothing to do (E <- E)

        clockCycles += 4;
        break;
    }

    case 0x5C:
    {
        // 92: LD E, H
        // Loads H into E
        // Flags affected: None

        mainBank.de.bytes.L = mainBank.hl.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x5D:
    {
        // 93: LD E, L
        // Loads H into E
        // Flags affected: None

        mainBank.de.bytes.L = mainBank.hl.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x5E:
    {
        // 94: LD E, (HL)
        // Loads content of memory pointed by HL into C
        // Flags affected: None

        mainBank.de.bytes.L = memory->peek(mainBank.hl.value);

        clockCycles += 4;
        break;
    }

    case 0x5F:
    {
        // 95: LD E, A
        // Loads A into E
        // Flags affected: None

        mainBank.de.bytes.L = mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x60:
    {
        // 96: LD H, B
        // Loads B into H
        // Flags affected: None

        mainBank.hl.bytes.H = mainBank.bc.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x61:
    {
        // 97: LD H, C
        // Loads C into H
        // Flags affected: None

        mainBank.hl.bytes.H = mainBank.bc.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x62:
    {
        // 98: LD H, D
        // Loads D into H
        // Flags affected: None

        mainBank.hl.bytes.H = mainBank.de.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x63:
    {
        // 99: LD H, E
        // Loads E into H
        // Flags affected: None

        mainBank.hl.bytes.H = mainBank.de.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x64:
    {
        // 100: LD H, H
        // Loads H into H
        // Flags affected: None

        // Nothing to do (H <- H)

        clockCycles += 4;
        break;
    }

    case 0x65:
    {
        // 101: LD H, L
        // Loads L into H
        // Flags affected: None

        mainBank.hl.bytes.H = mainBank.hl.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x66:
    {
        // 102: LD H, (HL)
        // Loads contents of memory pointed by HL into H
        // Flags affected: None

        mainBank.hl.bytes.H = memory->peek(mainBank.hl.value);

        clockCycles += 7;
        break;
    }

    case 0x67:
    {
        // 103: LD H, A
        // Loads A into H
        // Flags affected: None

        mainBank.hl.bytes.H = mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x68:
    {
        // 104: LD L, B
        // Loads B into L
        // Flags affected: None

        mainBank.hl.bytes.L = mainBank.bc.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x69:
    {
        // 105: LD L, C
        // Loads C into L
        // Flags affected: None

        mainBank.hl.bytes.L = mainBank.bc.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x6A:
    {
        // 106: LD L, D
        // Loads D into L
        // Flags affected: None

        mainBank.hl.bytes.L = mainBank.de.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x6B:
    {
        // 107: LD L, E
        // Loads E into L
        // Flags affected: None

        mainBank.hl.bytes.L = mainBank.de.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x6C:
    {
        // 108: LD L, H
        // Loads H into L
        // Flags affected: None

        mainBank.hl.bytes.L = mainBank.hl.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x6D:
    {
        // 109: LD L, L
        // Loads L into L
        // Flags affected: None

        // Nothing to do (L <- L)

        clockCycles += 4;
        break;
    }

    case 0x6E:
    {
        // 110: LD L, (HL)
        // Loads content of memory pointed by HL into L
        // Flags affected: None

        mainBank.hl.bytes.L = memory->peek(mainBank.hl.value);

        clockCycles += 4;
        break;
    }

    case 0x6F:
    {
        // 111: LD L, A
        // Loads A into L
        // Flags affected: None

        mainBank.hl.bytes.L = mainBank.af.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x70:
    {
        // 112: LD (HL), B
        // Store B register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.bc.bytes.H);
        clockCycles += 7;
        break;
    }

    case 0x71:
    {
        // 113: LD (HL), C
        // Store C register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.bc.bytes.L);
        clockCycles += 7;
        break;
    }

    case 0x72:
    {
        // 114: LD (HL), D
        // Store D register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.de.bytes.H);
        clockCycles += 7;
        break;
    }

    case 0x73:
    {
        // 115: LD (HL), E
        // Store E register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.de.bytes.L);
        clockCycles += 7;
        break;
    }

    case 0x74:
    {
        // 116: LD (HL), H
        // Store H register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.hl.bytes.H);
        clockCycles += 7;
        break;
    }

    case 0x75:
    {
        // 117: LD (HL), L
        // Store L register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.hl.bytes.L);
        clockCycles += 7;
        break;
    }

    case 0x76:
    {
        // 118: HALT

        // TODO
        // Implement suspension of CPU

        clockCycles += 4;
        break;
    }

    case 0x77:
    {
        // 119: LD (HL), A
        // Store A register into the position of memory located by HL register
        // Flags affected: None

        memory->poke(mainBank.hl.value, mainBank.af.bytes.H);
        clockCycles += 7;
        break;
    }

    case 0x78:
    {
        // 120: LD A, B
        // Loads B into A
        // Flags affected: None

        mainBank.af.bytes.H = mainBank.bc.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x79:
    {
        // 121: LD A, C
        // Loads C into A
        // Flags affected: None

        mainBank.af.bytes.H = mainBank.bc.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x7A:
    {
        // 122: LD A, D
        // Loads D into A
        // Flags affected: None

        mainBank.af.bytes.H = mainBank.de.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x7B:
    {
        // 123: LD A, E
        // Loads E into A
        // Flags affected: None

        mainBank.af.bytes.H = mainBank.de.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x7C:
    {
        // 124: LD A, H
        // Loads H into A
        // Flags affected: None

        mainBank.af.bytes.H = mainBank.hl.bytes.H;

        clockCycles += 4;
        break;
    }

    case 0x7D:
    {
        // 125: LD A, L
        // Loads L into A
        // Flags affected: None

        mainBank.af.bytes.H = mainBank.hl.bytes.L;

        clockCycles += 4;
        break;
    }

    case 0x7E:
    {
        // 126: LD A, (HL)
        // Loads the content of memory pointed by HL to A
        // Flags affected: None

        mainBank.af.bytes.H = memory->peek(mainBank.hl.value);

        clockCycles += 7;
        break;
    }

    case 0x7F:
    {
        // 127: LD A, A
        // Loads A into A
        // Flags affected: None

        // Nothing to do (A <- A)

        clockCycles += 4;
        break;
    }

    case 0x80:
    {
        // 128: ADD A, B
        // Adds B to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.bc), true);
        break;
    }

    case 0x81:
    {
        // 129: ADD A, C
        // Adds C to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.bc), false);
        break;
    }

    case 0x82:
    {
        // 130: ADD A, D
        // Adds D to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.de), true);
        break;
    }

    case 0x83:
    {
        // 131: ADD A, E
        // Adds E to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.de), false);
        break;
    }

    case 0x84:
    {
        // 132: ADD A, H
        // Adds H to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.hl), true);
        break;
    }

    case 0x86:
    {
        // 134: ADD A, (HL)
        // Adds contents of memory pointed by HL to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->add8(&(mainBank.af), true, value) + 3;
        break;
    }

    case 0x87:
    {
        // 135: ADD A, A
        // Adds A to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.af), true);
        break;
    }

    case 0x88:
    {
        // 136: ADC A, B
        // Adds B and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.bc), true, true);
        break;
    }

    case 0x89:
    {
        // 137: ADC A, C
        // Adds C and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.bc), false, true);
        break;
    }

    case 0x8A:
    {
        // 138: ADC A, D
        // Adds D and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.de), true, true);
        break;
    }

    case 0x8B:
    {
        // 139: ADC A, E
        // Adds E and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.de), false, true);
        break;
    }

    case 0x8C:
    {
        // 140: ADC A, H
        // Adds H and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.hl), true, true);
        break;
    }

    case 0x8D:
    {
        // 141: ADC A, L
        // Adds L and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.hl), false, true);
        break;
    }

    case 0x8E:
    {
        // 142: ADC A, (HL)
        // Adds contents of memory pointed by HL and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->add8(&(mainBank.af), true, value, true) + 3;
        break;
    }

    case 0x8F:
    {
        // 143: ADC A, A
        // Adds A and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.af), true, true);
        break;
    }

    case 0x90:
    {
        // 144: SUB B
        // Subtracts B from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.bc), true, false);
        break;
    }

    case 0x91:
    {
        // 145: SUB C
        // Subtracts C from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.bc), false, false);
        break;
    }

    case 0x92:
    {
        // 146: SUB D
        // Subtracts C from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.de), true, false);
        break;
    }

    case 0x93:
    {
        // 147: SUB E
        // Subtracts E from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.de), false, false);
        break;
    }

    case 0x94:
    {
        // 148: SUB H
        // Subtracts H from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.hl), true, false);
        break;
    }

    case 0x95:
    {
        // 149: SUB L
        // Subtracts L from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.hl), false, false);
        break;
    }

    case 0x96:
    {
        // 150: SUB (HL)
        // Subtracts contents of memory pointed by HL from A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->sub8(&(mainBank.af), true, value, false) + 3;
        break;
    }

    case 0x97:
    {
        // 151: SUB A
        // Subtracts A from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.af), true, false);
        break;
    }

    case 0x98:
    {
        // 152: SBC A, B
        // Subtracts B and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.bc), true, true);
        break;
    }

    case 0x99:
    {
        // 153: SBC A, C
        // Subtracts C and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.bc), false, true);
        break;
    }

    case 0x9A:
    {
        // 154: SBC A, D
        // Subtracts D and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.de), true, true);
        break;
    }

    case 0x9B:
    {
        // 155: SBC A, E
        // Subtracts E and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.de), false, true);
        break;
    }

    case 0x9C:
    {
        // 156: SBC A, H
        // Subtracts H and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.hl), true, true);
        break;
    }

    case 0x9D:
    {
        // 157: SBC A, L
        // Subtracts L and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.hl), false, true);
        break;
    }

    case 0x9E:
    {
        // 158: SBC A, (HL)
        // Subtracts contents of memory pointed by HL and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->sub8(&(mainBank.af), true, value, true) + 3;
        break;
    }

    case 0x9F:
    {
        // 159: SBC A, A
        // Subtracts A and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.af), true, true);
        break;
    }

    case 0xA0:
    {
        // 160: AND B
        // AND operation from B to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.bc), true);
        break;
    }

    case 0xA1:
    {
        // 161: AND C
        // AND operation from C to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.bc), false);
        break;
    }

    case 0xA2:
    {
        // 162: AND D
        // AND operation from D to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.de), true);
        break;
    }

    case 0xA3:
    {
        // 163: AND E
        // AND operation from E to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.de), false);
        break;
    }

    case 0xA4:
    {
        // 164: AND H
        // AND operation from H to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.hl), true);
        break;
    }

    case 0xA5:
    {
        // 165: AND L
        // AND operation L to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.hl), false) + 3;
        break;
    }

    case 0xA6:
    {
        // 166: AND (HL)
        // AND operation from contents of memory pointed by HL to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->and8(&(mainBank.af), true, value) + 3;
        break;
    }

    case 0xA7:
    {
        // 167: AND A
        // AND operation from A to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, &(mainBank.af), true) + 3;
        break;
    }

    case 0xA8:
    {
        // 168: XOR B
        // XOR operation from B to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.bc), true);
        break;
    }

    case 0xA9:
    {
        // 169: XOR C
        // XOR operation from C to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.bc), false);
        break;
    }

    case 0xAA:
    {
        // 170: XOR D
        // XOR operation from D to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.de), true);
        break;
    }

    case 0xAB:
    {
        // 171: XOR E
        // XOR operation from E to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.de), false);
        break;
    }

    case 0xAC:
    {
        // 172: XOR H
        // XOR operation from H to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.hl), true);
        break;
    }

    case 0xAD:
    {
        // 173: XOR L
        // XOR operation L to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.hl), false) + 3;
        break;
    }

    case 0xAE:
    {
        // 174: XOR (HL)
        // XOR operation from contents of memory pointed by HL to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->xor8(&(mainBank.af), true, value) + 3;
        break;
    }

    case 0xAF:
    {
        // 175: XOR A
        // XOR operation from A to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->xor8(&(mainBank.af), true, &(mainBank.af), true);
        break;
    }

    case 0xB0:
    {
        // 176: OR B
        // OR operation from B to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.bc), true);
        break;
    }

    case 0xB1:
    {
        // 177: OR C
        // OR operation from C to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.bc), false);
        break;
    }

    case 0xB2:
    {
        // 178: OR D
        // OR operation from D to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.de), true);
        break;
    }

    case 0xB3:
    {
        // 179: OR E
        // OR operation from E to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.de), false);
        break;
    }

    case 0xB4:
    {
        // 180: OR H
        // OR operation from H to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.hl), true);
        break;
    }

    case 0xB5:
    {
        // 181: OR L
        // OR operation from L to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.hl), false);
        break;
    }

    case 0xB6:
    {
        // 182: OR (HL)
        // OR operation from contents of memory pointed by HL to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->or8(&(mainBank.af), true, value) + 3;
        break;
    }

    case 0xB7:
    {
        // 183: OR A
        // OR operation from A to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->or8(&(mainBank.af), true, &(mainBank.af), true);
        break;
    }

    case 0xB8:
    {
        // 184: CP B
        // Change flags according subtraction between B and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.bc), true);
        break;
    }

    case 0xB9:
    {
        // 185: CP C
        // Change flags according subtraction between C and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.bc), false);
        break;
    }

    case 0xBA:
    {
        // 186: CP D
        // Change flags according subtraction between D and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.de), true);
        break;
    }

    case 0xBB:
    {
        // 187: CP E
        // Change flags according subtraction between E and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.de), false);
        break;
    }

    case 0xBC:
    {
        // 188: CP H
        // Change flags according subtraction between H and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.hl), true);
        break;
    }

    case 0xBD:
    {
        // 189: CP L
        // Change flags according subtraction between L and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.hl), false);
        break;
    }

    case 0xBE:
    {
        // 190: CP (HL)
        // Change flags acding subtration between content of memory pointed by HL and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(mainBank.hl.value);
        clockCycles += alu->cp8(&(mainBank.af), true, value) + 3;
        break;
    }

    case 0xBF:
    {
        // 191: CP A
        // Change flags according subtraction between A and A without by modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, &(mainBank.af), true);
        break;
    }

    case 0xC0:
    {
        // 192: RET NZ
        // Pop the content of memory pointed by SP into PC if FLAG_Z is 0
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_Z) == 0);
        break;
    }

    case 0xC1:
    {
        // 193: POP BC
        // Pop the content of memory pointed by SP into BC
        // Flags affected: None

        mainBank.bc.value = memory->peek(sp.value++) + (memory->peek(sp.value++) << 8);

        clockCycles += 10;
        break;
    }

    case 0xC2:
    {
        // 194: JP NZ, **
        // Set PC value to ** if FLAG_Z is 0
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_Z) == 0);
        break;
    }

    case 0xC3:
    {
        // 195: JP **
        // Set PC value to **
        // Flags affected: None

        clockCycles += jp(true);
        break;
    }

    case 0xC4:
    {
        // 196: CALL NZ, **
        // If FLAG_Z is 0:
        //  - Store PC (+3) onto the stack
        //	- Set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_Z) == 0);
        break;
    }

    case 0xC5:
    {
        // 197: PUSH BC
        // Push the content of BC register into the memory location pointed by SP
        // Flags affected: None

        memory->poke(--sp.value, mainBank.bc.bytes.H);
        memory->poke(--sp.value, mainBank.bc.bytes.L);

        clockCycles += 11;
        break;
    }

    case 0xC6:
    {
        // 199: ADD A, *
        // ADD * to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(pc.value++);
        clockCycles += alu->add8(&(mainBank.af), true, value);

        clockCycles += 3;
        break;
    }

    case 0xC7:
    {
        // 200: RST 00h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 0
        // Flags affected: C, N, P, H, Z, S

        clockCycles += rst(0);
        break;
    }

    case 0xC8:
    {
        // 201: RET Z
        // Pop the content of memory pointed by SP into PC if FLAG_Z is 1
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_Z) == 1);
        break;
    }

    case 0xC9:
    {
        // 201: RET
        // Pop the content of memory pointed by SP into PC
        // Flags affected: None

        pc.value = memory->peek(sp.value++) + (memory->peek(sp.value++) << 8);

        clockCycles += 10;
        break;
    }

    case 0xCA:
    {
        // 202: JP Z, **
        // Set PC value to ** if FLAG_Z is 1
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_Z) == 1);
        break;
    }

    case 0xCB:
    {
        // 203: BITS instructions

        // TODO
        // Implement bits operation (rotate, shift...)

        break;
    }

    case 0xCC:
    {
        // 204: CALL Z, **
        // if FLAG_Z is 1:
        //	- Push PC value (+3) into the memory location pointed by SP and set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_Z) == 1);
        break;
    }

    case 0xCD:
    {
        // 205: CALL **
        // Push PC value (+3) into the memory location pointed by SP and set PC value to **
        // Flags affected: None

        clockCycles += call(true);
        break;
    }

    case 0xCE:
    {
        // 206: ADC A, *
        // Adds D and carry flag to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(pc.value++);
        clockCycles += alu->add8(&(mainBank.af), true, &(mainBank.de), true, true);

        clockCycles += 3;
        break;
    }

    case 0xCF:
    {
        // 207: RST 08h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 8
        // Flags affected: C, N, P, H, Z, S

        clockCycles += rst(0x08);
        break;
    }

    case 0xD0:
    {
        // 208: RET NC
        // Pop the content of memory pointed by SP into PC if FLAG_C is 0
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_C) == 0);
        break;
    }

    case 0xD1:
    {
        // 209: POP DE
        // Pop the content of memory pointed by SP into DE
        // Flags affected: None

        mainBank.de.value = memory->peek(sp.value++) + (memory->peek(sp.value++) << 8);

        clockCycles += 10;
        break;
    }

    case 0xD2:
    {
        // 210: JP NC, **
        // Set PC value to ** if FLAG_C is 0
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_C) == 0);
        break;
    }

    case 0xD3:
    {
        // 211: OUT (**), A
        // Write value of A to port **
        // Flags affected: None

        pc.value++;

        clockCycles += 11;

        // TODO
        // Implement ports

        break;
    }

    case 0xD4:
    {
        // 212: CALL NC, **
        // If FLAG_C is 0:
        //  - Store PC (+3) onto the stack
        //	- Set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_C) == 0);
        break;
    }

    case 0xD5:
    {
        // 213: PUSH DE
        // Push the content of DE register into the memory location pointed by SP
        // Flags affected: None

        memory->poke(--sp.value, mainBank.de.bytes.H);
        memory->poke(--sp.value, mainBank.de.bytes.L);

        clockCycles += 11;
        break;
    }

    case 0xD6:
    {
        // 214: SUB A, *
        // SUB * to A
        // Flags affected: C, N, P, H, Z, S

        char value = (char)memory->peek(pc.value++);
        clockCycles += alu->sub8(&(mainBank.af), true, value);

        clockCycles += 3;
        break;
    }

    case 0xD7:
    {
        // 215: RST 10h
        // The current value of PC is pushed in SP and then is loaded with 0x10
        // Flags affected: None

        clockCycles += rst(0x10);
        break;
    }

    case 0xD8:
    {
        // 216: RET C
        // Pop the content of memory pointed by SP into PC if FLAG_C is 1
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_C) == 1);
        break;
    }

    case 0xD9:
    {
        // 217: EXX
        // Exchange the values of the registers BC, DE, HL by BD', DE', HL'
        // Flags affected: None

        auto bc = mainBank.bc;
        auto de = mainBank.de;
        auto hl = mainBank.hl;

        mainBank.bc = alternateBank.bc;
        mainBank.de = alternateBank.de;
        mainBank.hl = alternateBank.hl;

        alternateBank.bc = bc;
        alternateBank.de = de;
        alternateBank.hl = hl;

        clockCycles += 4;
        break;
    }

    case 0xDA:
    {
        // 218: JP C, **
        // Set PC value to ** if FLAG_C is 1
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_C) == 1);
        break;
    }

    case 0xDB:
    {
        // 219: IN A, N
        // A byte from a port N is written to register A
        // Flags affected: None

        clockCycles += 11;
        break;
    }

    case 0xDC:
    {
        // 220: CALL C, **
        // If FLAG_C is 1:
        //  - Store PC (+3) onto the stack
        //	- Set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_C) == 1);
        break;
    }

    case 0xDD:
    {
        // TODO
        // Implement IX Instructions

        break;
    }

    case 0xDE:
    {
        // 222: SBC A, N
        // Subtracts B and carry flag from A
        // Flags affected: C, N, P, H, Z, S

        auto value = memory->peek(pc.value++);
        clockCycles += alu->sub8(&(mainBank.af), true, &(mainBank.bc), true, true);
        clockCycles += 3;
        break;
    }

    case 0xDF:
    {
        // 223: RST 18h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 24
        // Flags affected: C, N, P, H, Z, S

        clockCycles += rst(0x18);
        break;
    }

    case 0xE0:
    {
        // 224: RET PO
        // Pop the content of memory pointed by SP into PC if FLAG_C is 1
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_C) == 1);
        break;
    }

    case 0xE1:
    {
        // 225: POP HL
        // Pop the content of memory pointed by SP into BC
        // Flags affected: None

        mainBank.hl.value = memory->peek(sp.value++) + (memory->peek(sp.value++) << 8);

        clockCycles += 10;
        break;
    }

    case 0xE2:
    {
        // 226: JP PO, **
        // Set PC value to ** if FLAG_P is 0
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_P) == 0);
        break;
    }

    case 0xE3:
    {
        // 227: EX (SP), HL
        // Exchanges the contents of (SP) and HL

        uint16_t value = (memory->peek(sp.value + 1) << 8) + memory->peek(sp.value);
        memory->poke(sp.value, mainBank.hl.bytes.L);
        memory->poke(sp.value + 1, mainBank.hl.bytes.H);
        mainBank.hl.value = value;

        clockCycles += 19;
        break;
    }

    case 0xE4:
    {
        // 228: CALL PO, **
        // If FLAG_P is 0:
        //  - Store PC (+3) onto the stack
        //	- Set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_P) == 0);
        break;
    }

    case 0xE5:
    {
        // 229: PUSH HL
        // Push the content of HL register into the memory location pointed by SP
        // Flags affected: None

        memory->poke(--sp.value, mainBank.hl.bytes.H);
        memory->poke(--sp.value, mainBank.hl.bytes.L);

        clockCycles += 11;
        break;
    }

    case 0xE6:
    {
        // 230: AND *
        // AND operation A to A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->and8(&(mainBank.af), true, memory->peek(pc.value++)) + 3;
        break;
    }

    case 0xE7:
    {
        // 231: RST 20h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 32
        // Flags affected: C, N, P, H, Z, S

        clockCycles += rst(0x20);
        break;
    }

    case 0xE8:
    {
        // 232: RET PE
        // Pop the content of memory pointed by SP into PC if FLAG_P is 0
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_P) == 0);
        break;
    }

    case 0xE9:
    {
        // 233: JP (HL)
        // Set PC value to contents of memory pointed by HL register
        // Flags affected: None

        auto value = memory->peek(mainBank.hl.value);
        pc.value = value;

        clockCycles += 10;
        break;
    }

    case 0xEA:
    {
        // 234: JP PE, **
        // Set PC value to ** if FLAG_P is 1
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_P) == 1);
        break;
    }

    case 0xEB:
    {
        // 235: EX DE, HL
        // Exchanges the contents of DE and HL

        uint16_t de = mainBank.af.value;
        mainBank.de.value = mainBank.hl.value = de;

        clockCycles += 4;
        break;
    }

    case 0xEC:
    {
        // 236: CALL PE, **
        // If FLAG_P is 1:
        //  - Store PC (+3) onto the stack
        //	- Set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_P) == 1);
        break;
    }

    case 0xED:
    {
        // TODO
        // Implement Misc instructions

        break;
    }

    case 0xEE:
    {
        // 238: XOR **
        // XOR operation from ** to A
        // Flags affected: C, N, P, H, Z, S

        auto value = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        clockCycles += alu->xor8(&(mainBank.af), true, value) + 3;
        break;
    }

    case 0xEF:
    {
        // 239: RST 28h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 40 (0x28)
        // Flags affected: C, N, P, H, Z, S

        clockCycles += rst(0x28);
        break;
    }

    case 0xF0:
    {
        // 240: RET P
        // Pop the content of memory pointed by SP into PC if FLAG_S is 0
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_S) == 0);
        break;
    }

    case 0xF1:
    {
        // 241: POP BC
        // Pop the content of memory pointed by SP into BC
        // Flags affected: None

        mainBank.bc.value = memory->peek(sp.value++) + (memory->peek(sp.value++) << 8);

        clockCycles += 10;
        break;
    }

    case 0xF2:
    {
        // 242: JP P, **
        // Set PC value to ** if FLAG_S is 0
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_S) == 0);
        break;
    }

    case 0xF3:
    {
        /// 243: DI
        /// Disable interrupts temporarily
        /// Flags affected: None

        // TODO: Implement the logic to disable interrupts
        // Note: This involves updating the interrupt control state

        break;
    }

    case 0xF4:
    {
        // 244: CALL P, **
        // If FLAG_S is 0:
        //  - Store PC (+3) onto the stack
        //	- Set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_S) == 0);
        break;
    }

    case 0xF5:
    {
        // 245: PUSH AF
        // Push the content of AF register into the memory location pointed by SP
        // Flags affected: None

        memory->poke(--sp.value, mainBank.af.bytes.H);
        memory->poke(--sp.value, mainBank.af.bytes.L);

        clockCycles += 11;
        break;
    }

    case 0xF6:
    {
        // 246: OR **
        // OR operation from ** to A
        // Flags affected: C, N, P, H, Z, S

        auto value = memory->peek(pc.value++) + (memory->peek(pc.value++) << 8);
        clockCycles += alu->xor8(&(mainBank.af), true, value) + 3;
    }

    case 0xF7:
    {
        // 247: RST 30h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 48 (0x30)
        // Flags affected: C, N, P, H, Z, S

        clockCycles += rst(0x30);
        break;
    }

    case 0xF8:
    {
        // 248: RET M
        // Pop the content of memory pointed by SP into PC if FLAG_S is 1
        // Flags affected: None

        clockCycles += ret(mainBank.getFlag(Flag::FLAG_S) == 0);
        break;
    }

    case 0xF9:
    {
        // 249: LD SP, HL
        // Loads HL into SP
        // Flags affected: None

        sp.value = mainBank.hl.value;

        clockCycles += 6;
        break;
    }

    case 0xFA:
    {
        // 250: JP M, **
        // Set PC value to ** if FLAG_S is 1
        // Flags affected: None

        clockCycles += jp(mainBank.getFlag(Flag::FLAG_S) == 1);
        break;
    }

    case 0xFB:
    {
        // 251: EI
        // Mask interruptions temporarily
        // Flags affected: None

        // TODO: Implement a method enableInterrupts

        clockCycles += 4;
        break;
    }

    case 0xFC:
    {
        // 252: CALL M, **
        // if FLAG_S is 1:
        //	- Push PC value (+3) into the memory location pointed by SP and set PC value to **
        // Flags affected: None

        clockCycles += call(mainBank.getFlag(Flag::FLAG_S) == 1);
        break;
    }

    case 0xFD:
    {
        // TODO: Implement IY Instructions
    }

    case 0xFE:
    {
        // 254: CP **
        // Change flags according subtraction between ** and A without modifying A
        // Flags affected: C, N, P, H, Z, S

        clockCycles += alu->cp8(&(mainBank.af), true, memory->peek(pc.value++));
        clockCycles += 3;
        break;
    }

    case 0xFF:
    {
        // 255: RST 38h
        // Push PC value (+1) into the memory location pointed by SP and set PC value to 0x38
        // Flags affected:  None

        clockCycles += rst(0x38);
        break;
    }

    default:
    {
        break;
    }
    }

    return 0;
}

} // namespace emuzeta80
