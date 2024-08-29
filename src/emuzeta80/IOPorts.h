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
 * @file IOPorts.cpp
 * @ingroup emuzeta80
 * @author Lumpi (iflumpi@gmail.com)
 * @version 0.1
 *
 * @brief Emulation of I/O ports 
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

class IOPorts
{
public:
    IOPorts();

    void addDevice(uint16_t port);
    uint8_t read(uint16_t port);
    void write(uint16_t port, uint8_t value);

protected:
    std::map<uint16_t, uint8_t> devices;
};

} // namespace emuzeta80
