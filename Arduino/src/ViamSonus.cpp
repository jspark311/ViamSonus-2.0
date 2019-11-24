/*
File:   ViamSonus.cpp
Author: J. Ian Lindsay
Date:   2019.11.18


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

#include "ViamSonus.h"
#include <string.h>


const char* const ViamSonus::errorToStr(ViamSonusError err) {
  switch (err) {
    case ViamSonusError::INPUT_DISPLACED:   return "INPUT_DISPLACED";
    case ViamSonusError::DEVICE_DISABLED:   return "DEVICE_DISABLED";
    case ViamSonusError::PEGGED_MAX:        return "PEGGED_MAX";
    case ViamSonusError::PEGGED_MIN:        return "PEGGED_MIN";
    case ViamSonusError::NO_ERROR:          return "NO_ERROR";
    case ViamSonusError::ABSENT:            return "ABSENT";
    case ViamSonusError::BUS:               return "BUS";
    case ViamSonusError::ALREADY_AT_MAX:    return "ALREADY_AT_MAX";
    case ViamSonusError::ALREADY_AT_MIN:    return "ALREADY_AT_MIN";
    case ViamSonusError::INVALID_POT:       return "INVALID_POT";
    case ViamSonusError::UNROUTE_FAILED:    return "UNROUTE_FAILED";
    case ViamSonusError::BAD_COLUMN:        return "BAD_COLUMN";
    case ViamSonusError::BAD_ROW:           return "BAD_ROW";
    case ViamSonusError::SWITCH_COLLISION:  return "SWITCH_COLLISION";
    case ViamSonusError::GEN_SWITCH_FAULT:  return "GEN_SWITCH_FAULT";
    default:                                return "UNKNOWN";
  }
}


/*
* Constructor. Here is all of the setup work. Takes the i2c addresses of the hardware as arguments.
*/
ViamSonus::ViamSonus(const uint8_t reset_pin) :
  cp_switch(0x70, reset_pin),
  pot0(0x28), pot1(0x29), pot2(0x2A), pot3(0x2B), pot4(0x2C), pot5(0x2D)
{
  for (uint8_t i = 0; i < 12; i++) {   // Setup our input channels.
    inputs[i].name      = nullptr;
    inputs[i].i_chan    = i;
    inputs[i].o_chans   = 0;
    inputs[i].deadspace = 0;
    inputs[i].flags     = 0;
  }
  for (uint8_t i = 0; i < 8; i++) {    // Setup our output channels.
    outputs[i].name      = nullptr;
    outputs[i].i_chans   = 0;
    outputs[i].o_chan    = i;
    outputs[i].flags     = 0;
  }
}

/*
* Destructor
*/
ViamSonus::~ViamSonus() {
  // Destroy the objects that represeent our hardware. Downstream operations will
  //   put the hardware into an inert state, so that needn't be done here.
  for (uint8_t i = 0; i < 12; i++) {
    if (inputs[i].name) {
      free(inputs[i].name);
    }
  }
  for (uint8_t i = 0; i < 8; i++) {
    if (outputs[i].name) {
      free(outputs[i].name);
    }
  }
}


/*
* Do all the bus-related init.
*/
ViamSonusError ViamSonus::init() {
  for (uint8_t i = 0; i < 6; i++) {
    DIGITALPOT_ERROR result = _getPotRef(i)->init();
    if (result != DIGITALPOT_ERROR::NO_ERROR) {
      //local_log.concatf("Failed to init() pot %u with cause (%d).\n", i, result);
      return ViamSonusError::ABSENT;
    }
  }
  int8_t result = (int8_t) cp_switch.init();
  if (result != 0) {
    //local_log.concatf("Failed to init() cp_switch with cause (%d).\n", result);
    return ViamSonusError::ABSENT;
  }

  // If we are this far, it means we've successfully refreshed all the device classes
  //   to reflect the state of the hardware. Now to parse that data into structs that
  //   mean something to us at this level...
  for (uint8_t i = 0; i < 12; i++) {  // Routes...
    uint8_t temp_byte = cp_switch.getValue(inputs[i].i_chan);
    for (uint8_t j = 0; j < 8; j++) {
      if (0x01 & temp_byte) {
        CPOutputChannel* temp_output = getOutputByCol(j);
        temp_output->i_chans |= 1 << i;
      }
      temp_byte = temp_byte >> 1;
    }
  }

  return ViamSonusError::NO_ERROR;
}


ViamSonusError ViamSonus::reset() {
  ViamSonusError ret = ViamSonusError::GEN_SWITCH_FAULT;
  ADG2128_ERROR res = cp_switch.reset();
  if (ADG2128_ERROR::NO_ERROR == res) {
    ret = ViamSonusError::NO_ERROR;
  }
  return ret;
}


/*
*/
ViamSonusError ViamSonus::nameInput(uint8_t row, const char* name) {
  if (row > 11) return ViamSonusError::BAD_ROW;
  int len = strlen(name);
  if (inputs[row].name) {
    free(inputs[row].name);
  }
  inputs[row].name = (char *) malloc(len + 1);
  for (int i = 0; i < len; i++) *(inputs[row].name + i) = *(name + i);
  return ViamSonusError::NO_ERROR;
}


/*
*/
ViamSonusError ViamSonus::nameOutput(uint8_t col, const char* name) {
  if (col > 7) return ViamSonusError::BAD_COLUMN;
  int len = strlen(name);
  if (outputs[col].name) {
    free(outputs[col].name);
  }
  outputs[col].name = (char *) malloc(len + 1);
  for (int i = 0; i < len; i++) *(outputs[col].name + i) = *(name + i);
  return ViamSonusError::NO_ERROR;
}



ViamSonusError ViamSonus::unroute(uint8_t col, uint8_t row) {
  if (col > 7)  return ViamSonusError::BAD_COLUMN;
  if (row > 11) return ViamSonusError::BAD_ROW;
  bool remove_link = (outputs[col].i_chans & (1 << row));
  ViamSonusError ret = ViamSonusError::NO_ERROR;
  if (ADG2128_ERROR::NO_ERROR != cp_switch.unsetRoute(outputs[col].o_chan, row)) {
    ret = ViamSonusError::UNROUTE_FAILED;
  }
  if (remove_link) {
    outputs[col].i_chans &= ~(1 << row);
  }
  return ret;
}


ViamSonusError ViamSonus::unroute(uint8_t col) {
  if (col > 7)  return ViamSonusError::BAD_COLUMN;
  ViamSonusError ret = ViamSonusError::NO_ERROR;
  for (int i = 0; i < 12; i++) {
    if (unroute(col, i) != ViamSonusError::NO_ERROR) {
      return ViamSonusError::UNROUTE_FAILED;
    }
  }
  return ret;
}


/*
* Remember: This is the class responsible for ensuring that we don't cross-wire a circuit.
*   The crosspoint switch class is generalized, and knows nothing about the circuit it is
*   embedded within. It will follow whatever instructions it is given. So if we tell it
*   to route two inputs to the same output, it will oblige and possibly fry hastily-built
*   hardware. So under that condition, we unroute prior to routing and return a code to
*   indicate that we've done so.
*/
ViamSonusError ViamSonus::route(uint8_t col, uint8_t row) {
  if (col > 7)  return ViamSonusError::BAD_COLUMN;
  if (row > 11) return ViamSonusError::BAD_ROW;

  ViamSonusError ret = ViamSonusError::NO_ERROR;
  if (0 != outputs[col].i_chans) {
    ViamSonusError result = unroute(col);
    if (result == ViamSonusError::NO_ERROR) {
      ret = ViamSonusError::INPUT_DISPLACED;
      outputs[col].i_chans &= ~(1 << row);
    }
    else {
      ret = ViamSonusError::UNROUTE_FAILED;
    }
  }

  if (((uint8_t)ret) >= 0) {
    ADG2128_ERROR result = cp_switch.setRoute(outputs[col].o_chan, row);
    switch (result) {
      case ADG2128_ERROR::NO_ERROR:
        outputs[col].i_chans |= (1 << row);
        ret = ViamSonusError::NO_ERROR;
        break;
      case ADG2128_ERROR::ABSENT:
        ret = ViamSonusError::ABSENT;
        break;
      case ADG2128_ERROR::BUS:
        ret = ViamSonusError::BUS;
        break;
      case ADG2128_ERROR::BAD_COLUMN:
        ret = ViamSonusError::BAD_COLUMN;
        break;
      case ADG2128_ERROR::BAD_ROW:
        ret = ViamSonusError::BAD_ROW;
        break;
      case ADG2128_ERROR::NO_MEM:
      ret = ViamSonusError::GEN_SWITCH_FAULT;
        break;
      case ADG2128_ERROR::CARD_VIOLATION_COL:
      case ADG2128_ERROR::CARD_VIOLATION_ROW:
        ret = ViamSonusError::SWITCH_COLLISION;
        break;
    }
  }
  return ret;
}


ViamSonusError ViamSonus::setVolume(uint8_t row, uint8_t vol) {
  ViamSonusError ret = ViamSonusError::NO_ERROR;
  if (row > 11)  return ViamSonusError::BAD_ROW;
  ret = (ViamSonusError) _getPotRef(row)->setValue(row & 0x01, vol);
  return ret;
}


int16_t ViamSonus::getVolume(uint8_t row) {
  if (row > 11)  return 0;
  return (int16_t) _getPotRef(row)->getValue(row & 0x01);
}



void ViamSonus::printDebug(StringBuilder* output) {
  output->concat("ViamSonus 2.0\n");
}


void ViamSonus::dumpOutputChannel(uint8_t chan, StringBuilder* output) {
  if (chan > 7)  {
    output->concat("dumpOutputChannel() was passed an out-of-bounds id.\n");
    return;
  }
  output->concatf("Output channel %u\n", chan);

  if (outputs[chan].name) {
    output->concatf("%s\n", outputs[chan].name);
  }
  output->concatf("Switch column %u\n", outputs[chan].o_chan);
  if (outputs[chan].i_chans == 0) {
    output->concatf("Output channel %u is presently unbound.\n", chan);
  }
  else {
    output->concatf("Output channel %u is presently bound to the following inputs...\n", chan);
    for (uint8_t i = 0; i < 12; i++) {
      if ((1 << i) & outputs[chan].i_chans) {
        dumpInputChannel(&inputs[i], output);
      }
    }
  }
}


void ViamSonus::dumpInputChannel(CPInputChannel *chan, StringBuilder* output) {
  if (chan == nullptr) {
    output->concat("dumpInputChannel() was passed a NULL InputChannel.\n");
    return;
  }
  if (chan->name) {
    output->concatf("%s\n", chan->name);
  }
  else {
    output->concatf("Input channel %u\n", chan);
  }
  output->concatf("Switch row: %u\n", chan->i_chan);
  DS1881* pot = _getPotRef(chan->i_chan);
  output->concatf("Potentiometer value:      %u\n", pot->getValue(chan->i_chan & 1));
}


void ViamSonus::dumpInputChannel(uint8_t chan, StringBuilder* output) {
  if (chan > 11) {
    output->concatf("dumpInputChannel() was passed an out-of-bounds input channel (%u).\n", chan);
    return;
  }
  dumpInputChannel(&inputs[chan], output);
}


/*
* No bounds-checking.
*/
DS1881* ViamSonus::_getPotRef(uint8_t row) {
  DS1881* refs[] = {&pot0, &pot1, &pot2, &pot3, &pot4, &pot5};
  return refs[row >> 1];
}
