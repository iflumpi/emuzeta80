
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
 * @brief RAM class for simulating a memory of a computer
 *
 */

//-------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------

#include "RAM.h"

//-------------------------------------------------------------------------
// Class implementation
//-------------------------------------------------------------------------

namespace emuzeta80
{

/**
 * @brief RAM class constructor
 *
 * This constructor initializes an instance of the RAM class with the specified size
 * The size parameter determines the total capacity of the RAM in bytes
 *
 * @param size The size of the RAM in bytes
 */
RAM::RAM(uint64_t size)
{
    this->size = size;
}

/**
 * @brief Peeks at the byte located in the RAM at the given position
 *
 * This function allows retrieving the value of a byte stored in the RAM
 * at the specified memory position without altering the RAM contents.
 *
 * @param position The memory position to peek at.
 * @return The byte value located at the specified memory position.
 */
uint8_t RAM::peek(uint64_t position)
{
    if(content.count(position) > 0)
        return content[position];

    return 0;
}

/**
 * @brief Pokes a byte into the RAM at the specified position
 *
 * This function allows writing a byte value into the RAM at the provided memory
 * position. The content of the RAM at the given position will be updated with
 * the new value
 *
 * @param position The memory position where the byte will be written
 * @param value The byte value to be written into the RAM
 */
void RAM::poke(uint64_t position, uint8_t value)
{
    content[position] = value;
}

} // namespace emuzeta80
