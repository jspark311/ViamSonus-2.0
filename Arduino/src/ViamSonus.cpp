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


/*
* Constructor. Here is all of the setup work. Takes the i2c addresses of the hardware as arguments.
*/
ViamSonus::ViamSonus(I2CAdapter* i2c, uint8_t cp_addr, uint8_t dp_lo_addr, uint8_t dp_hi_addr) : {
  const ADG2128Opts cp_opts(
    cp_addr,  // Device address
    255,      // No hard reset pin.
    true,     // Allow two sinks for a given source.
    false     // Disallow two sources from sharing a sink.
  );

  cp_switch = new ADG2128(&cp_opts);

  for (uint8_t i = 0; i < 12; i++) {   // Setup our input channels.
    inputs[i].cp_row   = i;
    inputs[i].name     = nullptr;
  }

  for (uint8_t i = 0; i < 8; i++) {    // Setup our output channels.
    outputs[i].cp_column = col_remap[i];
    outputs[i].i_chan    = nullptr;
    outputs[i].name      = nullptr;
    outputs[i].high_pot  = i < 4;
    outputs[i].dp_reg    = i % 4;
  }
}

ViamSonus::~ViamSonus() {
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
  // Destroy the objects that represeent our hardware. Downstream operations will
  //   put the hardware into an inert state, so that needn't be done here.
}


/*
* Do all the bus-related init.
*/
ViamSonusError ViamSonus::init() {
  const DS1881* refs[] = {&pot0, &pot1, &pot2, &pot3, &pot4, &pot5};
  for (uint8_t i = 0; i < 6; i++) {
    int8_t result = refs[i]->init();
    if (result != 0) {
      //local_log.concatf("Failed to init() dp_lo with cause (%d).\n", result);
      return ViamSonusError::BUS;
    }
  }
  int8_t result = (int8_t) cp_switch->init();
  if (result != 0) {
    //local_log.concatf("Failed to init() cp_switch with cause (%d).\n", result);
    return ViamSonusError::BUS;
  }

  // If we are this far, it means we've successfully refreshed all the device classes
  //   to reflect the state of the hardware. Now to parse that data into structs that
  //   mean something to us at this level...
  for (int i = 0; i < 12; i++) {  // Routes...
    uint8_t temp_byte = cp_switch->getValue(inputs[i].cp_row);
    for (int j = 0; j < 8; j++) {
      if (0x01 & temp_byte) {
        CPOutputChannel* temp_output = getOutputByCol(j);
        if (temp_output) {
          temp_output->i_chan = &inputs[i];
        }
      }
      temp_byte = temp_byte >> 1;
    }
  }

  return ViamSonusError::NO_ERROR;
}


CPOutputChannel* ViamSonus::getOutputByCol(uint8_t col) {
  if (col > 7)  return nullptr;
  for (int j = 0; j < 8; j++) {
    if (outputs[j].cp_column == col) {
      return &outputs[j];
    }
  }
  return nullptr;
}


/*
*/
ViamSonusError ViamSonus::nameInput(uint8_t row, char* name) {
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
ViamSonusError ViamSonus::nameOutput(uint8_t col, char* name) {
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
  bool remove_link = (outputs[col].i_chan == &inputs[row]) ? true : false;
  uint8_t return_value = ViamSonusError::NO_ERROR;
  if (0 > (int8_t) cp_switch->unsetRoute(outputs[col].cp_column, row)) {
    return ViamSonusError::UNROUTE_FAILED;
  }
  if (remove_link) outputs[col].i_chan = nullptr;
  return return_value;
}


ViamSonusError ViamSonus::unroute(uint8_t col) {
  if (col > 7)  return ViamSonusError::BAD_COLUMN;
  uint8_t return_value = ViamSonusError::NO_ERROR;
  for (int i = 0; i < 12; i++) {
    if (unroute(col, i) != ViamSonusError::NO_ERROR) {
      return ViamSonusError::UNROUTE_FAILED;
    }
  }
  return return_value;
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

  int8_t return_value = ViamSonusError::NO_ERROR;
  if (outputs[col].i_chan) {
    int8_t result = unroute(col);
    if (result == ViamSonusError::NO_ERROR) {
      return_value = ViamSonusError::INPUT_DISPLACED;
      outputs[col].i_chan = nullptr;
    }
    else {
      return_value = ViamSonusError::UNROUTE_FAILED;
    }
  }

  if (return_value >= 0) {
    int8_t result = (int8_t) cp_switch->setRoute(outputs[col].cp_column, row);
    if (result != ViamSonusError::NO_ERROR) {
      return_value = result;
    }
    else {
      outputs[col].i_chan = &inputs[row];
    }
  }

  return return_value;
}


ViamSonusError ViamSonus::setVolume(uint8_t row, uint8_t vol) {
  int8_t return_value = ViamSonusError::NO_ERROR;
  if (row > 11)  return ViamSonusError::BAD_ROW;
  return_value = (int8_t) _getPotRef(row)->setValue(row & 0x01, vol);
  return return_value;
}


int16_t ViamSonus::getVolume(uint8_t row) {
  if (row > 11)  return ViamSonusError::BAD_ROW;
  return (int16_t) _getPotRef(row)->getValue(row & 0x01);
}



void ViamSonus::dumpOutputChannel(uint8_t chan, StringBuilder* output) {
  if (chan > 7)  {
    output->concat("dumpOutputChannel() was passed an out-of-bounds id.\n");
    return;
  }
  output->concatf("Output channel %d\n", chan);

  if (outputs[chan].name) {
    output->concatf("%s\n", outputs[chan].name);
  }
  output->concatf("Switch column %d\n", outputs[chan].cp_column);
  if (_getPotRef(outputs[chan].high_pot) == nullptr) {
    output->concatf("Potentiometer is NULL\n");
  }
  else {
    output->concatf("Potentiometer:            %u\n", (outputs[chan].high_pot) ? 1 : 0);
    output->concatf("Potentiometer register:   %u\n", outputs[chan].dp_reg);
    output->concatf("Potentiometer value:      %u\n", _getPotRef(outputs[chan].high_pot)->getValue(outputs[chan].dp_reg));
  }
  if (outputs[chan].i_chan == nullptr) {
    output->concatf("Output channel %d is presently unbound.\n", chan);
  }
  else {
    output->concatf("Output channel %d is presently bound to the following input...\n", chan);
    dumpInputChannel(outputs[chan].i_chan, output);
  }
}


void ViamSonus::dumpInputChannel(CPInputChannel *chan, StringBuilder* output) {
  if (chan == nullptr) {
    Kernel::log("dumpInputChannel() was passed an out-of-bounds id.\n");
    return;
  }

  if (chan->name) {
    output->concatf("%s\n", chan->name);
  }
  output->concatf("Switch row: %d\n", chan->cp_row);
}


void ViamSonus::dumpInputChannel(uint8_t chan, StringBuilder* output) {
  if (chan > 11) {
    output->concatf("dumpInputChannel() was passed an out-of-bounds id.\n");
    return;
  }
  output->concatf("Input channel %d\n", chan);
  if (inputs[chan].name) {
    output->concatf("%s\n", inputs[chan].name);
  }
  output->concatf("Switch row: %d\n", inputs[chan].cp_row);
}

/*
* No bounds-checking.
*/
DS1881* ViamSonus::_getPotRef(uint8_t row) {
  const DS1881* refs[] = {&pot0, &pot1, &pot2, &pot3, &pot4, &pot5};
  return (refs[row >> 1]);
}
