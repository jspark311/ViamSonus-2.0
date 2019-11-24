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
#include <StringBuilder.h>


#define VIAMSONUS_SERIALIZE_VERSION  1


enum class ViamSonusError : int8_t {
  INPUT_DISPLACED  = 4,   // There was no error, but a channel-routing operation has displaced a previously-routed input.
  DEVICE_DISABLED  = 3,   // A caller tried to set a wiper while the device is disabled. This may work...
  PEGGED_MAX       = 2,   // There was no error, but a call to change a wiper setting pegged the wiper at its highest position.
  PEGGED_MIN       = 1,   // There was no error, but a call to change a wiper setting pegged the wiper at its lowest position.
  NO_ERROR         = 0,   // There was no error.
  ABSENT           = -1,  // A part on the board appears to not be connected to the bus.
  BUS              = -2,  // Something went wrong with the i2c bus.
  ALREADY_AT_MAX   = -3,  // A caller tried to increase the value of the wiper beyond its maximum.
  ALREADY_AT_MIN   = -4,  // A caller tried to decrease the value of the wiper below its minimum.
  INVALID_POT      = -5,  // There aren't that many potentiometers.
  UNROUTE_FAILED   = -6,  // We tried to unroute a signal from an output and failed.
  BAD_COLUMN       = -7,  // Column was out-of-bounds.
  BAD_ROW          = -8,  // Row was out-of-bounds.
  SWITCH_COLLISION = -9,  // Command would violate a row or col cardinality constraint.
  GEN_SWITCH_FAULT = -10  // A general switch failure.
};


// This struct defines an input pin on the PCB.
typedef struct cps_input_channel_t {
  char*    name;       // A name for this input. Not required.
  uint8_t  i_chan;     // The row in the crosspoint switch associated with this input.
  uint8_t  o_chans;    // A flag field indicating which columns are presently bound to this row.
  uint8_t  deadspace;  // Structure padding.
  uint8_t  flags;      // Flags on this channel.
} CPInputChannel;


// This struct defines an output pin on the PCB.
typedef struct cps_output_channel_t {
  char*    name;       // A name for this output. Not required.
  uint16_t i_chans;    // A flag field indicating which rows are presently bound to this column.
  uint8_t  o_chan;     // The column in the crosspoint switch associated with this output.
  uint8_t  flags;      // Flags on this channel.
} CPOutputChannel;



/*
* This class represents a complete Viam Sonus board.
* The 8-pin group are the outputs, and the 12-pin group are inputs. Please see
*   the schematic for more clarity.
*/
class ViamSonus {
  public:
    ViamSonus(const uint8_t reset_pin);
    ~ViamSonus();

    ViamSonusError init();
    ViamSonusError reset();

    ViamSonusError route(uint8_t col, uint8_t row);       // Establish a route to the given output from the given input.
    ViamSonusError unroute(uint8_t col, uint8_t row);     // Disconnect the given output from the given input.
    ViamSonusError unroute(uint8_t col);                  // Disconnect the given output from all inputs.

    ViamSonusError nameInput(uint8_t row, const char*);   // Name the input channel.
    ViamSonusError nameOutput(uint8_t col, const char*);  // Name the output channel.

    ViamSonusError setVolume(uint8_t row, uint8_t vol);   // Set the volume of a given input channel.
    int16_t getVolume(uint8_t row);                       // Get the volume of a given input channel.

    // Save this hardware state into a buffer for later restoration.
    ViamSonusError serialize(uint8_t* buf, unsigned int* len);

    void printDebug(StringBuilder*);
    void dumpInputChannel(CPInputChannel* chan, StringBuilder*);
    void dumpInputChannel(uint8_t chan, StringBuilder*);
    void dumpOutputChannel(uint8_t chan, StringBuilder*);

    inline CPInputChannel*  getInputByRow(uint8_t row) {   return &inputs[row % 12];     };
    inline CPOutputChannel* getOutputByCol(uint8_t col) {  return &outputs[col & 0x07];  };

    static const char* const errorToStr(ViamSonusError);


  private:
    CPInputChannel  inputs[12];
    CPOutputChannel outputs[8];

    ADG2128 cp_switch;
    DS1881  pot0;
    DS1881  pot1;
    DS1881  pot2;
    DS1881  pot3;
    DS1881  pot4;
    DS1881  pot5;

    DS1881* _getPotRef(uint8_t row);
};

#endif   // __VIAMSONUS_DRIVER_H__
