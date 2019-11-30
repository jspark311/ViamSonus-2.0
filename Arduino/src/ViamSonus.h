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
#define VIAMSONUS_SERIALIZE_SIZE     (5 + ADG2128_SERIALIZE_SIZE + (DS1881_SERIALIZE_SIZE*6))

/* ViamSonus class flags */
#define VIAMSONUS_FLAG_PRESERVE_STATE 0x01000000  // Preserve hardware states.
#define VIAMSONUS_FLAG_FOUND_SWITCH   0x02000000  // Found the switch.
#define VIAMSONUS_FLAG_FOUND_POT_0    0x04000000  // Found this potentiometer.
#define VIAMSONUS_FLAG_FOUND_POT_1    0x08000000  // Found this potentiometer.
#define VIAMSONUS_FLAG_FOUND_POT_2    0x10000000  // Found this potentiometer.
#define VIAMSONUS_FLAG_FOUND_POT_3    0x20000000  // Found this potentiometer.
#define VIAMSONUS_FLAG_FOUND_POT_4    0x40000000  // Found this potentiometer.
#define VIAMSONUS_FLAG_FOUND_POT_5    0x80000000  // Found this potentiometer.

#define VIAMSONUS_FLAG_RESET_MASK     0xFE000000  // Bits to preserve through reset.
#define VIAMSONUS_FLAG_ALL_DEVS_MASK  0xFE000000  // All devices found.
#define VIAMSONUS_FLAG_SERIAL_MASK    0x01000000  // Only these bits are serialized.


/* InputChannel flags */
#define VSIC_FLAG_ALLOW_MIX           0x0001  // This channel is safe to mix.
#define VSIC_FLAG_MONITOR             0x0002  // Monitor this channel via ADC.


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
  uint16_t flags;      // Flags on this channel.
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
    ViamSonus(const uint8_t* buf, const unsigned int len);
    ~ViamSonus();

    ViamSonusError init();
    ViamSonusError reset();
    ViamSonusError refresh();

    ViamSonusError route(uint8_t col, uint8_t row);       // Establish a route to the given output from the given input.
    ViamSonusError unroute(uint8_t col, uint8_t row);     // Disconnect the given output from the given input.
    ViamSonusError unroute(uint8_t col);                  // Disconnect the given output from all inputs.

    ViamSonusError nameInput(uint8_t row, const char*);   // Name the input channel.
    ViamSonusError nameOutput(uint8_t col, const char*);  // Name the output channel.

    ViamSonusError setVolume(uint8_t row, uint8_t vol);   // Set the volume of a given input channel.
    int16_t getVolume(uint8_t row);                       // Get the volume of a given input channel.

    // Save this hardware state into a buffer for later restoration.
    uint32_t serialize(uint8_t* buf, unsigned int len);
    ViamSonusError preserveOnDestroy(bool);

    void printDebug(StringBuilder*);
    void dumpInputChannel(CPInputChannel* chan, StringBuilder*);
    void dumpInputChannel(uint8_t chan, StringBuilder*);
    void dumpOutputChannel(uint8_t chan, StringBuilder*);

    inline CPInputChannel*  getInputByRow(uint8_t row) {   return &inputs[row % 12];     };
    inline CPOutputChannel* getOutputByCol(uint8_t col) {  return &outputs[col & 0x07];  };

    inline bool allDevsFound() {
      return (VIAMSONUS_FLAG_ALL_DEVS_MASK == (_flags & VIAMSONUS_FLAG_ALL_DEVS_MASK));
    };

    static const char* const errorToStr(ViamSonusError);


  private:
    CPInputChannel  inputs[12];
    CPOutputChannel outputs[8];
    uint32_t _flags = 0;

    ADG2128 cp_switch;
    DS1881  pot0;
    DS1881  pot1;
    DS1881  pot2;
    DS1881  pot3;
    DS1881  pot4;
    DS1881  pot5;

    DS1881* _getPotRef(uint8_t row);
    int8_t _unserialize(const uint8_t* buf, const unsigned int len);

    inline bool _preserve_on_destroy() {
      return _vs_flag(VIAMSONUS_FLAG_PRESERVE_STATE);
    };

    /* Flag manipulation inlines */
    inline uint32_t _vs_flags() {                return _flags;           };
    inline bool _vs_flag(uint32_t _flag) {       return (_flags & _flag); };
    inline void _vs_clear_flag(uint32_t _flag) { _flags &= ~_flag;        };
    inline void _vs_set_flag(uint32_t _flag) {   _flags |= _flag;         };
    inline void _vs_set_flag(uint32_t _flag, bool nu) {
      if (nu) _flags |= _flag;
      else    _flags &= ~_flag;
    };
};

#endif   // __VIAMSONUS_DRIVER_H__
