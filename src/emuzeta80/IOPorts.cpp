
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

#include "IOPorts.h"

//-------------------------------------------------------------------------
// Class implementation
//-------------------------------------------------------------------------

namespace emuzeta80
{

/**
 * @brief IOPorts class constructor
 */
IOPorts::IOPorts()
{
}

/*
 * @brief Add a new device associated to a port
 */
void IOPorts::addDevice(uint16_t port)
{
    devices[port] = 0;    
}


/**
 * @brief Read the value of an I/O port
 */
uint8_t IOPorts::read(uint16_t port)
{
    if(devices.count(port) > 0)
        return devices[port]; 

    return 0;
}

/**
 * @brief Write a value into a port
 */
void IOPorts::write(uint16_t port, uint8_t value)
{
    devices[port] = value;
}

} // namespace emuzeta80
