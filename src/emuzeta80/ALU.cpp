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
 * @file ALU.cpp
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

#include "ALU.h"

//-------------------------------------------------------------------------
// Class implementation
//-------------------------------------------------------------------------

namespace emuzeta80
{

ALU::ALU(RegistersBank* mainBank)
{
    this->mainBank = mainBank;
}

/**
 * @brief Increase value of a 8-bit register by one
 *
 * This function increments an 8-bit register in the CPU.
 * The register can be either the high or low byte of a 16-bit register.
 * Flags affected: N, P, H, Z, S
 *
 * @param reg16 pointer to the 16-bit register.
 * @param high boolean value indicating whether to increment the high byte or the low byte of the register.
 * @return uint16_t number of cycles to execute the operation
 */
uint16_t ALU::inc8(Register* reg16, bool high)
{
    uint8_t value = high ? reg16->bytes.H : reg16->bytes.L;
    uint8_t updatedValue = value + 1;
    if(high)
        reg16->bytes.H = updatedValue;
    else
        reg16->bytes.L = updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_P, value == 0x7F);
    mainBank->setFlag(FLAG_H, value == 0x0F);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue == 0x80);

    return 4;
}

/**
 * @brief Decrease value of a 8-bit register by one
 *
 * This function decreases an 8-bit register in the CPU.
 * The register can be either the high or low byte of a 16-bit register.
 * Flags affected: N, P, H, Z, S
 *
 * @param reg16 pointer to the 16-bit register.
 * @param high boolean value indicating whether to decrease the high byte or the low byte of the register.
 * @return uint16_t number of cycles to execute the operation
 */
uint16_t ALU::dec8(Register* reg8, bool high)
{
    uint8_t value = high ? reg8->bytes.H : reg8->bytes.L;
    uint8_t updatedValue = value - 1;
    if(high)
        reg8->bytes.H = updatedValue;
    else
        reg8->bytes.L = updatedValue;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_P, value == 0x80); // -128 = 0x80 in 2a complement
    mainBank->setFlag(FLAG_H, updatedValue == 0x0F);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue == 0x7F);

    return 4;
}

/**
 * @brief Adds value of a 8-bit register to another 8-bit register
 *
 * Flags affected: C, N, P, H, Z, S
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param reg8B target register (bc, de, hl, af)
 * @param highB flag specify if select the higher or lower byte of the second register
 * @param carry true if an additional carry is considered in the addition
 *
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::add8(Register* reg8A, bool highA, Register* reg8B, bool highB, bool carry)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueB = highB ? reg8B->bytes.H : reg8B->bytes.L;
    auto valueC = (carry & mainBank->getFlag(FLAG_C)) ? 1 : 0;
    uint16_t updatedValue = (uint8_t)valueA + (uint8_t)valueB + (uint8_t)valueC;
    if(highA)
        reg8A->bytes.H = (uint8_t)updatedValue;
    else
        reg8A->bytes.L = (uint8_t)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) != 0);
    mainBank->setFlag(FLAG_C, updatedValue > 0xFF);
    mainBank->setFlag(FLAG_H, (valueA & 0x0F) + (valueB & 0x0F) > 0x0F);
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128);

    return 4;
}

/**
 * @brief Adds value to a 8-bit register
 *
 * This function adds the specified 8-bit value to the given register
 * If the carry flag is set, it adds an additional 1 to the result
 *
 * Flags affected: C, N, P, H, Z, S
 *
 * @param reg8A pointer to the register to add the value to
 * @param highA flag indicating whether the high or low byte of the register should be modified
 * @param value 8-bit value to add to the register
 * @param carry flag indicating whether to include an additional carry in the addition
 *
 * @return number of cycles of the operation
 */
uint16_t ALU::add8(Register* reg8A, bool highA, char value, bool carry)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueC = carry & mainBank->getFlag(FLAG_C) ? 1 : 0;
    int16_t updatedValue = (char)valueA + value + valueC;
    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, updatedValue > 0xFF);
    mainBank->setFlag(FLAG_H, (valueA & 0x0F) + (value & 0x0F) > 0x0F);
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128);

    return 4;
}

/**
 * @brief Subtracts the value of REG8B to REG8B
 *
 * Flags affected: C, N, P, H, Z, S
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param reg8B target register (bc, de, hl, af)
 * @param highB flag specify if select the higher or lower byte of the target register
 * @param carry true if an additional carry is considered in the substraction
 *
 * @return number of cycles of the operation
 */
uint16_t ALU::sub8(Register* reg8A, bool highA, Register* reg8B, bool highB, bool carry)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueB = highB ? reg8B->bytes.H : reg8B->bytes.L;
    auto valueC = (carry & mainBank->getFlag(FLAG_C)) ? 1 : 0;
    int16_t updatedValue = (char)valueA - (char)valueB - (char)valueC;
    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, (updatedValue & 0xFF00) > 0);               // TODO
    mainBank->setFlag(FLAG_H, (valueA & 0x0F) + (valueB & 0x0F) > 0x0F);  // TODO (valueB + c?)
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128); // TODO

    return 4;
}

/**
 * @brief Subtracts the value of VALUE to REG8B
 *
 * Flags affected: C, N, P, H, Z, S
 * 
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param value value to substract to the source register
 * @param carry true if an additional carry is considered in the substraction
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::sub8(Register* reg8A, bool highA, char value, bool carry)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueC = (carry & mainBank->getFlag(FLAG_C)) ? 1 : 0;
    int16_t updatedValue = (char)valueA - value - valueC;
    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, (updatedValue & 0xFF00) > 0);               // TODO
    mainBank->setFlag(FLAG_H, (valueA & 0x0F) + (value & 0x0F) > 0x0F);   // TODO (valueB + c?)
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128); // TODO

    return 4;
}

/**
 * @brief AND operation between two registers
 *
 * Flags affected: C, N, P, H, Z, S
 * 
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param reg8B target register ((bc, de, hl, af)
 * @param highB flag specify if select the higher or lower byte of the target register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::and8(Register* reg8A, bool highA, Register* reg8B, bool highB)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueB = highB ? reg8B->bytes.H : reg8B->bytes.L;
    uint8_t updatedValue = valueA & valueB;

    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) > 0);
    mainBank->setFlag(FLAG_C, false);
    mainBank->setFlag(FLAG_H, true);
    mainBank->setFlag(FLAG_P, true); // TODO

    return 4;
}

/**
 * @brief AND operation between a register and a value
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param value 8-bit value to AND with register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::and8(Register* reg8A, bool highA, char value)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    uint8_t updatedValue = valueA & value;

    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) > 0);
    mainBank->setFlag(FLAG_C, false);
    mainBank->setFlag(FLAG_H, true);
    mainBank->setFlag(FLAG_P, true); // TODO

    return 4;
}

/**
 * @brief OR operation between two registers
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param reg8B target register ((bc, de, hl, af)
 * @param highB flag specify if select the higher or lower byte of the target register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::or8(Register* reg8A, bool highA, Register* reg8B, bool highB)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueB = highB ? reg8B->bytes.H : reg8B->bytes.L;
    uint8_t updatedValue = valueA | valueB;

    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) > 0);
    mainBank->setFlag(FLAG_C, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_P, true); // TODO

    return 4;
}

/**
 * @brief OR operation between a register and a value
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param value 8-bit value to OR with register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::or8(Register* reg8A, bool highA, char value)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    uint8_t updatedValue = valueA | value;

    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) > 0);
    mainBank->setFlag(FLAG_C, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_P, true); // TODO

    return 4;
}

/**
 * @brief XOR operation between two registers
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param reg8B target register ((bc, de, hl, af)
 * @param highB flag specify if select the higher or lower byte of the target register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::xor8(Register* reg8A, bool highA, Register* reg8B, bool highB)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueB = highB ? reg8B->bytes.H : reg8B->bytes.L;
    uint8_t updatedValue = valueA ^ valueB;

    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) > 0);
    mainBank->setFlag(FLAG_C, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_P, true); // TODO

    return 4;
}

/**
 * @brief XOR operation between a register and a value
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param value 8-bit value to XOR with register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::xor8(Register* reg8A, bool highA, char value)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    uint8_t updatedValue = valueA ^ value;

    if(highA)
        reg8A->bytes.H = (char)updatedValue;
    else
        reg8A->bytes.L = (char)updatedValue;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) > 0);
    mainBank->setFlag(FLAG_C, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_P, true); // TODO

    return 4;
}

/**
 * @brief Compares two 8-bit values without modifying them
 *
 * This function compares the contents of two 8-bit registers without modifying
 * their values. It performs a comparison similar to a subtraction but discards
 * the result. The zero flag (Z) and carry flag (C) may be affected based on the
 * comparison result
 *
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param reg8B target register ((bc, de, hl, af)
 * @param highB flag specify if select the higher or lower byte of the target register
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::cp8(Register* reg8A, bool highA, Register* reg8B, bool highB)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    auto valueB = highB ? reg8B->bytes.H : reg8B->bytes.L;
    int16_t updatedValue = (char)valueA - (char)valueB;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, (updatedValue & 0xFF00) > 0);               // TODO
    mainBank->setFlag(FLAG_H, (valueA & 0x0F) + (valueB & 0x0F) > 0x0F);  // TODO (valueB + c?)
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128); // TODO

    return 4;
}

/**
 * @brief Compares an 8-bit register with a given 8-bit value without modifying them
 *
 * This function compares the contents of an 8-bit register and an 8-bit value without
 * modifying their values. It performs a comparison similar to a subtraction but discards
 * the result. The zero flag (Z) and carry flag (C) may be affected based on the
 * comparison result
 * 
 * @param reg8A source register (bc, de, hl, af)
 * @param highA flag to specify if select the higher or lower byte of the source register
 * @param value 8-bit value for comparison
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::cp8(Register* reg8A, bool highA, char value)
{
    auto valueA = highA ? reg8A->bytes.H : reg8A->bytes.L;
    int16_t updatedValue = (char)valueA - value;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, (updatedValue & 0xFF00) > 0);               // TODO
    mainBank->setFlag(FLAG_H, (valueA & 0x0F) + (value & 0x0F) > 0x0F);   // TODO (valueB + c?)
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128); // TODO

    return 4;
}

/**
 * @brief Adds two 16-bit values and updates the result in the specified register.
 *
 * This function adds the contents of two 16-bit registers and updates the result in
 * the specified register. The carry flag (C) may be affected based on the addition result.
 * 
 * @param reg16A source register (bc, de, hl, af)
 * @param reg16B target register ((bc, de, hl, af)
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::add16(Register* reg16A, Register* reg16B)
{
    // Adds the value of REG16B to REG16A
    // Flags affected: C, N, H

    uint16_t value = reg16A->value;
    uint16_t updatedValue = reg16A->value + reg16B->value;
    reg16A->value = updatedValue;

    // TODO: Add Carry Bit?

    mainBank->setFlag(FLAG_C, value + reg16B->value > 0xFF);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, (reg16A->value & 0x0F) + (reg16B->value & 0x0F) > 0x0F);

    return 11;
}

} // namespace emuzeta80