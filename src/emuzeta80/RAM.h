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

#include <cstdint>
#include <map>

//-------------------------------------------------------------------------
// Class definition
//-------------------------------------------------------------------------

namespace emuzeta80
{

class RAM
{
public:
    RAM(uint64_t size);

    uint8_t peek(uint64_t position);
    void poke(uint64_t position, uint8_t value);

protected:
    uint64_t size;
    std::map<uint64_t, uint8_t> content;
};

} // namespace emuzeta80
