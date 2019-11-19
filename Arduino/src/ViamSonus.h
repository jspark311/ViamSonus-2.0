/*
File:   ViamSonus.h
Author: J. Ian Lindsay
Date:   2019.11.16

Copyright 2019 Manuvr, Inc

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Digipots start at i2c address 0x28, and there are 6 of them.
Crosspoint switch is at address 0x70.
*/

#ifndef __VIAMSONUS_DRIVER_H__
#define __VIAMSONUS_DRIVER_H__

#include <Arduino.h>
#include <DS1881.h>
#include <ADG2128.h>


enum class VIAMSONUS_ERROR : int8_t {
  INPUT_DISPLACED = 4,   // There was no error, but a channel-routing operation has displaced a previously-routed input.
  DEVICE_DISABLED = 3,   // A caller tried to set a wiper while the device is disabled. This may work...
  PEGGED_MAX      = 2,   // There was no error, but a call to change a wiper setting pegged the wiper at its highest position.
  PEGGED_MIN      = 1,   // There was no error, but a call to change a wiper setting pegged the wiper at its lowest position.
  NO_ERROR        = 0,   // There was no error.
  ABSENT          = -1,  // A part on the board appears to not be connected to the bus.
  BUS             = -2,  // Something went wrong with the i2c bus.
  ALREADY_AT_MAX  = -3,  // A caller tried to increase the value of the wiper beyond its maximum.
  ALREADY_AT_MIN  = -4,  // A caller tried to decrease the value of the wiper below its minimum.
  INVALID_POT     = -5,  // There aren't that many potentiometers.
  UNROUTE_FAILED  = -6,  // We tried to unroute a signal from an output and failed.
  BAD_COLUMN      = -7,  // Column was out-of-bounds.
  BAD_ROW         = -8   // Row was out-of-bounds.
};


// This struct defines an input pin on the PCB.
typedef struct cps_input_channel_t {
  char*     name;             // A name for this input.
  uint8_t   cp_row;
} CPInputChannel;


// This struct defines an output pin on the PCB.
typedef struct cps_output_channel_t {
  char*           name;          // A name for this output.
  uint8_t         cp_column;
  CPInputChannel* cp_row;        // A pointer to the row that is presently bound to this column. If NULL, it is unbound.
} CPOutputChannel;



class AudioRouter {
  public:
    AudioRouter(uint8_t, uint8_t, uint8_t);       // Constructor needs the i2c addresses of the three chips on the PCB.
    ~AudioRouter(void);

    int8_t init(void);
    void preserveOnDestroy(bool);

    int8_t route(uint8_t col, uint8_t row);       // Establish a route to the given output from the given input.

    int8_t unroute(uint8_t col, uint8_t row);     // Disconnect the given output from the given input.
    int8_t unroute(uint8_t col);                  // Disconnect the given output from all inputs.

    int8_t nameInput(uint8_t row, const char*);   // Name the input channel.
    int8_t nameOutput(uint8_t col, const char*);  // Name the output channel.

    int8_t setVolume(uint8_t col, uint8_t vol);   // Set the volume coming out of a given output channel.

    int8_t enable(void);      // Turn on the chips responsible for routing signals.
    int8_t disable(void);     // Turn off the chips responsible for routing signals.

    int8_t status(char*);     // Write some status about the routes to the provided char buffer.

    // TODO: These ought to be statics...
    void dumpInputChannel(CPInputChannel *chan);
    void dumpInputChannel(uint8_t chan);
    void dumpOutputChannel(uint8_t chan);


  private:
    CPInputChannel*  inputs[12];
    CPOutputChannel* outputs[8];

    CPOutputChannel* getOutputByCol(uint8_t);

    static const uint8_t col_remap[8];
};




/*
* This class represents a complete Viam Sonus board.
* The 8-pin group are the outputs, and the 12-pin group are inputs. Please see
*   the schematic for more clarity.
*/
class ViamSonus {
  public:
    ViamSonus(const uint8_t address);
    ~ViamSonus();

    void printDebug();
    VIAMSONUS_ERROR init();


  private:
    ADG2128 switch(0x70);
    DS1881  pot0(0x28);
    DS1881  pot1(0x29);
    DS1881  pot2(0x2A);
    DS1881  pot3(0x2B);
    DS1881  pot4(0x2C);
    DS1881  pot5(0x2D);
};

#endif   // __VIAMSONUS_DRIVER_H__
