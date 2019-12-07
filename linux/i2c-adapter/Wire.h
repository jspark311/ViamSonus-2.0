/*
File:   i2c-adapter.h
Author: J. Ian Lindsay
Date:   2014.03.10


Copyright (C) 2014 J. Ian Lindsay
All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef I2C_ABSTRACTION_LAYER         // This is meant to prevent double-inclusion.
#define I2C_ABSTRACTION_LAYER

#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>


class TwoWire {
  public:
    bool bus_error;

    TwoWire(const char*);  // Constructor takes a bus path as an argument.
    ~TwoWire();            // Destructor

    bool busIdle();          // Returns true if the bus is ready to service a transaction right now.
    bool busOnline();
    void setDebug(bool);

    int beginTransmission(uint8_t addr);
    int write(uint8_t byte);
    int endTransmission();
    int requestFrom(uint8_t addr, unsigned int bytes);
    uint8_t receive();


  private:
    bool bus_online;
    bool bus_in_use;
    bool debug;
    uint8_t last_used_bus_addr;
    int open_bus_descriptor;
    int writeX(uint8_t dev_addr, uint8_t sub_addr, uint16_t byte_count, uint8_t *buf);
    int readX(uint8_t, uint8_t, uint8_t, uint8_t*);
    bool switch_device(uint8_t);      // Call this to switch to another i2c device on the bus.
};

#endif
