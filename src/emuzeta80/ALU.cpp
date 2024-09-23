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
 * Flags affected: N, P, H, Z, S
 *
 * @param value pointer to the 8-bit register.
 * @return uint16_t number of cycles to execute the operation
 */
uint16_t ALU::inc8(uint8_t* value)
{
    uint8_t updatedValue = *value + 1;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_P, *value == 0x7F);
    mainBank->setFlag(FLAG_H, *value == 0x0F);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue == 0x80);

    *value = updatedValue;

    return 4;
}

/**
 * @brief Decrease value of a 8-bit register by one
 *
 * This function decreases an 8-bit register in the CPU.
 * Flags affected: N, P, H, Z, S
 *
 * @param value pointer to the 8-bit register.
 * @return uint16_t number of cycles to execute the operation
 */
uint16_t ALU::dec8(uint8_t* value)
{
    uint8_t updatedValue = *value - 1;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_P, *value == 0x7F);
    mainBank->setFlag(FLAG_H, *value == 0x0F);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue == 0x80);

    *value = updatedValue;

    return 4;
}

/**
 * @brief Adds value of a 8-bit register to another 8-bit register
 *
 * Flags affected: C, N, P, H, Z, S
 *
 * @param target target value register
 * @param source source value register
 * @param carry true if an additional carry is considered in the addition
 *
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::add8(uint8_t* target, uint8_t source, bool carry)
{
    uint8_t valuec = (carry & mainBank->getFlag(FLAG_C)) ? 1 : 0;
    uint16_t updatedValue = *target + source + valuec;

    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, (updatedValue & 0x80) != 0);
    mainBank->setFlag(FLAG_C, updatedValue > 0xFF);
    mainBank->setFlag(FLAG_H, (*target & 0x0F) + (source & 0x0F) > 0x0F);
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128);

    *target = updatedValue;

    return 4;
}

/**
 * @brief Subtracts the value of REG8B to REG8B
 *
 * Flags affected: C, N, P, H, Z, S
 *
 * @param target target value register
 * @param source source value register
 * @param carry true if an additional carry is considered in the substraction
 *
 * @return number of cycles of the operation
 */
uint16_t ALU::sub8(uint8_t* target, uint8_t source, bool carry)
{
    auto valuec = (carry & mainBank->getFlag(FLAG_C)) ? 1 : 0;
    int16_t updatedValue = (char)(*target) - (char)source - (char)valuec;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, (updatedValue & 0xFF00) > 0);               // TODO
    mainBank->setFlag(FLAG_H, (*target & 0x0F) + (source & 0x0F) > 0x0F); // TODO (valueB + c?)
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128); // TODO

    *target = updatedValue;

    return 4;
}

/**
 * @brief AND operation between two values
 *
 * Flags affected: C, N, P, H, Z, S
 *
 * @param target target value
 * @param value source value (target = target & value)
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::and8(uint8_t* target, uint8_t value)
{
    uint8_t updatedValue = *target & value;
    *target = updatedValue;

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
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::or8(uint8_t* target, uint8_t value)
{
    uint8_t updatedValue = *target | value;
    *target = updatedValue;

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
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::xor8(uint8_t* target, uint8_t value)
{    
    uint8_t updatedValue = *target ^ value;
    *target = updatedValue;

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
 * the result.
 *
 * @param reg1 first register
 * @param reg2 second register (cp -> reg1-reg2)
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::cp8(uint8_t reg1, uint8_t reg2)
{
    int16_t updatedValue = (char)reg1 - (char)reg2;

    mainBank->setFlag(FLAG_N, true);
    mainBank->setFlag(FLAG_Z, updatedValue == 0);
    mainBank->setFlag(FLAG_S, updatedValue < 0);
    mainBank->setFlag(FLAG_C, (updatedValue & 0xFF00) > 0);               // TODO
    mainBank->setFlag(FLAG_H, (reg1 & 0x0F) + (reg2 & 0x0F) > 0x0F);  // TODO (valueB + c?)
    mainBank->setFlag(FLAG_P, updatedValue > 127 || updatedValue < -128); // TODO

    return 4;
}

/**
 * @brief Adds two 16-bit values and updates the result in the specified register.
 *
 * This function adds the contents of two 16-bit registers and updates the result in
 * the specified register.
 *
 * @param reg16A source register (bc, de, hl, af)
 * @param reg16B target register ((bc, de, hl, af)
 * @return uint16_t number of cycles of the operation
 */
uint16_t ALU::add16(uint16_t* target, uint16_t source, bool carry)
{
    uint16_t previousValue = *target;    
    *target = *target + source + ((carry & mainBank->getFlag(FLAG_C)) ? 1 : 0);    

    // TODO: Add Carry Bit?
    mainBank->setFlag(FLAG_C, previousValue + source > 0xFF);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, (*target & 0x0F) + (source & 0x0F) > 0x0F);

    return 11;
}

uint16_t ALU::sub16(uint16_t* target, uint16_t source, bool carry)
{
    uint16_t previousValue = *target;
    *target = *target + source + ((carry & mainBank->getFlag(FLAG_C)) ? 1 : 0);    

    // TODO: Add Carry Bit?
    mainBank->setFlag(FLAG_C, previousValue + source > 0xFF);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, (*target & 0x0F) + (source & 0x0F) > 0x0F);

    return 11;
}

uint16_t ALU::rlc(uint8_t* value)
{
    auto msb = (*value & 0x80) == 0x80;
    auto updatedValue = (*value << 1) | (msb ? 0x01 : 0x00);
    *value = updatedValue;

    mainBank->setFlag(FLAG_C, msb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::rrc(uint8_t* value)
{
    auto lsb = (*value & 0x01) == 0x01;
    auto updatedValue = (*value >> 1) | (lsb ? 0x80 : 0x80);
    *value = updatedValue;

    mainBank->setFlag(FLAG_C, lsb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::rl(uint8_t* value)
{
    auto msb = (*value & 0x80) == 0x80;
    auto updatedValue = (*value << 1) | (mainBank->getFlag(FLAG_C) ? 0x01 : 0x00);
    *value = updatedValue;

    mainBank->setFlag(FLAG_C, msb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::rr(uint8_t* value)
{
    auto lsb = (*value & 0x80) == 0x80;
    auto updatedValue = (*value >> 1) | (mainBank->getFlag(FLAG_C) ? 0x80 : 0x80);
    *value = updatedValue;

    mainBank->setFlag(FLAG_C, lsb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::sla(uint8_t* value)
{
    auto msb = (*value & 0x80) == 0x80;
    *value = *value << 1;

    mainBank->setFlag(FLAG_C, msb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::sra(uint8_t* value)
{
    auto lsb = (*value & 0x01) == 0x01;
    *value = *value >> 1;

    mainBank->setFlag(FLAG_C, lsb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::sll(uint8_t* value)
{
    auto msb = (*value & 0x80) == 0x80;
    *value = (*value << 1) + 0x01;

    mainBank->setFlag(FLAG_C, msb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::srl(uint8_t* value)
{
    auto lsb = (*value & 0x01) == 0x01;
    *value = *value >> 1;

    mainBank->setFlag(FLAG_C, lsb);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, false);
    mainBank->setFlag(FLAG_Z, *value == 0);

    return 0;
}

uint16_t ALU::bit(uint8_t* value, uint8_t bitPosition)
{
    uint8_t mask = 1 << bitPosition;

    mainBank->setFlag(FLAG_Z, (*value & mask) != 0);
    mainBank->setFlag(FLAG_N, false);
    mainBank->setFlag(FLAG_H, true);
}

uint16_t ALU::res(uint8_t* value, uint8_t bitPosition)
{
    uint8_t mask = ~(1 << bitPosition);
    *value &= mask;

    return 8;
}

uint16_t ALU::set(uint8_t* value, uint8_t bitPosition)
{
    uint8_t mask = 1 << bitPosition;
    *value |= mask;

    return 8;
}

uint16_t ALU::neg(uint8_t* value)
{
    *value = ~(*value);

    // TODO update flags

    return 8;
}

} // namespace emuzeta80
