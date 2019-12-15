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
* Constructor. Here is all of the setup work. Takes the i2c addresses of the hardware as arguments.
*/
ViamSonus::ViamSonus(const uint8_t* buf, const unsigned int len) :
  cp_switch(0x70, *(buf+6)),
  pot0(0x28), pot1(0x29), pot2(0x2A), pot3(0x2B), pot4(0x2C), pot5(0x2D)
{
  for (uint8_t i = 0; i < 12; i++) {   // Setup our input channels.
    inputs[i].name      = nullptr;
    inputs[i].i_chan    = i;
    inputs[i].o_chans   = 0;
    inputs[i].flags     = 0;
  }
  for (uint8_t i = 0; i < 8; i++) {    // Setup our output channels.
    outputs[i].name      = nullptr;
    outputs[i].i_chans   = 0;
    outputs[i].o_chan    = i;
    outputs[i].flags     = 0;
  }
  unserialize(buf, len);
}



/*
* Destructor
*/
ViamSonus::~ViamSonus() {
  while (_pending_ops.hasNext()) {
    delete _pending_ops.get();
  }
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

  while (_inputs.size() > 0) {
    delete _inputs.dequeue();
  }
  while (_outputs.size() > 0) {
    delete _outputs.dequeue();
  }
}


/*
* Do all the bus-related init.
*/
ViamSonusError ViamSonus::init(TwoWire* bus) {
  ViamSonusError ret = ViamSonusError::ABSENT;
  _vs_clear_flag(VIAMSONUS_FLAG_ALL_DEVS_MASK);
  _vs_set_flag(VIAMSONUS_FLAG_FOUND_POT_0, (DIGITALPOT_ERROR::NO_ERROR == pot0.init(bus)));
  _vs_set_flag(VIAMSONUS_FLAG_FOUND_POT_1, (DIGITALPOT_ERROR::NO_ERROR == pot1.init(bus)));
  _vs_set_flag(VIAMSONUS_FLAG_FOUND_POT_2, (DIGITALPOT_ERROR::NO_ERROR == pot2.init(bus)));
  _vs_set_flag(VIAMSONUS_FLAG_FOUND_POT_3, (DIGITALPOT_ERROR::NO_ERROR == pot3.init(bus)));
  _vs_set_flag(VIAMSONUS_FLAG_FOUND_POT_4, (DIGITALPOT_ERROR::NO_ERROR == pot4.init(bus)));
  _vs_set_flag(VIAMSONUS_FLAG_FOUND_POT_5, (DIGITALPOT_ERROR::NO_ERROR == pot5.init(bus)));

  // TODO: Migrate this assurance into DS1881 driver.
  if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_0)) {   pot0.setRange(63);   }
  if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_1)) {   pot1.setRange(63);   }
  if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_2)) {   pot2.setRange(63);   }
  if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_3)) {   pot3.setRange(63);   }
  if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_4)) {   pot4.setRange(63);   }
  if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_5)) {   pot5.setRange(63);   }

  if (0 == (int8_t) cp_switch.init(bus)) {
    _vs_set_flag(VIAMSONUS_FLAG_FOUND_SWITCH);
    // If we are this far, it means we've successfully refreshed all the device classes
    //   to reflect the state of the hardware. Now to parse that data into structs that
    //   mean something to us at this level...
    for (uint8_t i = 0; i < 12; i++) {  // Routes...
      uint8_t temp_byte = cp_switch.getCols(inputs[i].i_chan);
      for (uint8_t j = 0; j < 8; j++) {
        if (0x01 & temp_byte) {
          CPOutputChannel* temp_output = getOutputByCol(j);
          temp_output->i_chans |= 1 << i;
        }
        temp_byte = temp_byte >> 1;
      }
    }
  }

  if (allDevsFound()) {
    ret = ViamSonusError::NO_ERROR;
  }
  return ret;
}


ViamSonusError ViamSonus::reset() {
  ViamSonusError ret = ViamSonusError::GEN_SWITCH_FAULT;
  ADG2128_ERROR res = cp_switch.reset();
  if (ADG2128_ERROR::NO_ERROR == res) {
    // Clear out our local records of what is connected where.
    for (uint8_t i = 0; i < 8; i++) {
      inputs[i].o_chans  = 0;
      outputs[i].i_chans = 0;
    }
    for (uint8_t i = 8; i < 12; i++) {
      inputs[i].o_chans  = 0;
    }
    ret = ViamSonusError::NO_ERROR;
  }
  return ret;
}


ViamSonusError ViamSonus::preserveOnDestroy(bool preserve) {
  ViamSonusError ret = ViamSonusError::GEN_SWITCH_FAULT;
  cp_switch.preserveOnDestroy(preserve);
  for (uint8_t i = 0; i < 6; i++) {
    if (DIGITALPOT_ERROR::NO_ERROR != _getPotRef(i)->storeWipers()) {
      return ret;
    }
    ret = ViamSonusError::NO_ERROR;
    _vs_set_flag(VIAMSONUS_FLAG_PRESERVE_STATE);
  }
  return ret;
}


ViamSonusError ViamSonus::refresh() {
  ViamSonusError ret = ViamSonusError::GEN_SWITCH_FAULT;
  ADG2128_ERROR res = cp_switch.refresh();
  if (ADG2128_ERROR::NO_ERROR == res) {
    DIGITALPOT_ERROR dp_ret = DIGITALPOT_ERROR::NO_ERROR;
    if ((DIGITALPOT_ERROR::NO_ERROR == dp_ret) & _vs_flag(VIAMSONUS_FLAG_FOUND_POT_0)) {  dp_ret = pot0.refresh();  }
    if ((DIGITALPOT_ERROR::NO_ERROR == dp_ret) & _vs_flag(VIAMSONUS_FLAG_FOUND_POT_1)) {  dp_ret = pot1.refresh();  }
    if ((DIGITALPOT_ERROR::NO_ERROR == dp_ret) & _vs_flag(VIAMSONUS_FLAG_FOUND_POT_2)) {  dp_ret = pot2.refresh();  }
    if ((DIGITALPOT_ERROR::NO_ERROR == dp_ret) & _vs_flag(VIAMSONUS_FLAG_FOUND_POT_3)) {  dp_ret = pot3.refresh();  }
    if ((DIGITALPOT_ERROR::NO_ERROR == dp_ret) & _vs_flag(VIAMSONUS_FLAG_FOUND_POT_4)) {  dp_ret = pot4.refresh();  }
    if ((DIGITALPOT_ERROR::NO_ERROR == dp_ret) & _vs_flag(VIAMSONUS_FLAG_FOUND_POT_5)) {  dp_ret = pot5.refresh();  }
    ret = (DIGITALPOT_ERROR::NO_ERROR != dp_ret) ? ViamSonusError::BUS : ViamSonusError::NO_ERROR;
  }
  return ret;
}


/*
*/
ViamSonusError ViamSonus::nameInput(uint8_t row, const char* name) {
  if (row > 11) return ViamSonusError::BAD_ROW;
  ViamSonusError ret = ViamSonusError::GEN_SWITCH_FAULT;
  int len = strlen(name);
  if (inputs[row].name) {
    free(inputs[row].name);
  }
  inputs[row].name = (char *) malloc(len + 1);
  if (nullptr != inputs[row].name) {
    *(inputs[row].name + len) = 0;
    for (int i = 0; i < len; i++) *(inputs[row].name + i) = *(name + i);
    ret = ViamSonusError::NO_ERROR;
  }
  return ret;
}


/*
*/
ViamSonusError ViamSonus::nameOutput(uint8_t col, const char* name) {
  if (col > 7) return ViamSonusError::BAD_COLUMN;
  ViamSonusError ret = ViamSonusError::GEN_SWITCH_FAULT;
  int len = strlen(name);
  if (outputs[col].name) {
    free(outputs[col].name);
  }
  outputs[col].name = (char *) malloc(len + 1);
  if (nullptr != outputs[col].name) {
    *(outputs[col].name + len) = 0;
    for (int i = 0; i < len; i++) *(outputs[col].name + i) = *(name + i);
    ret = ViamSonusError::NO_ERROR;
  }
  return ret;
}



ViamSonusError ViamSonus::unroute(uint8_t col, uint8_t row) {
  if (col > 7)  return ViamSonusError::BAD_COLUMN;
  if (row > 11) return ViamSonusError::BAD_ROW;
  ViamSonusError ret = ViamSonusError::UNROUTE_FAILED;
  if (ADG2128_ERROR::NO_ERROR == cp_switch.unsetRoute(col, row)) {
    outputs[col].i_chans &= ~(1 << row);
    inputs[row].o_chans  &= ~(1 << col);
    ret = ViamSonusError::NO_ERROR;
  }
  return ret;
}


ViamSonusError ViamSonus::unroute(uint8_t col) {
  if (col > 7)  return ViamSonusError::BAD_COLUMN;
  ViamSonusError ret = ViamSonusError::NO_ERROR;
  for (int i = 0; i < 12; i++) {
    if (ADG2128_ERROR::NO_ERROR == cp_switch.unsetRoute(col, i, (11 == i))) {
      outputs[col].i_chans &= ~(1 << i);
      inputs[i].o_chans    &= ~(1 << col);
    }
    else {
      ret = ViamSonusError::UNROUTE_FAILED;
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
    // We already have channels bound.
    if (0 != (outputs[col].i_chans & ~(1 << row))) {
      // There are routes attached beyond the one requested.
      // TODO: Implications for mix after this line.
      if (ViamSonusError::NO_ERROR == unroute(col)) {
        ret = ViamSonusError::INPUT_DISPLACED;
      }
      else {
        ret = ViamSonusError::UNROUTE_FAILED;
      }
    }
    else {
      // The requested route already exists.
    }
  }

  if (((int8_t)ret) >= 0) {
    ADG2128_ERROR result = cp_switch.setRoute(outputs[col].o_chan, row);
    switch (result) {
      case ADG2128_ERROR::NO_ERROR:
        outputs[col].i_chans |= (1 << row);
        inputs[row].o_chans  |= (1 << col);
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
      default:
        ret = ViamSonusError::GEN_SWITCH_FAULT;
        break;
    }
  }
  return ret;
}


ViamSonusError ViamSonus::setVolume(uint8_t row, uint8_t vol) {
  if (row > 11)  return ViamSonusError::BAD_ROW;
  ViamSonusError ret = (ViamSonusError) _getPotRef(row)->setValue(row & 0x01, vol);
  return ret;
}


int16_t ViamSonus::getVolume(uint8_t row) {
  if (row > 11)  return 0;
  return (int16_t) _getPotRef(row)->getValue(row & 0x01);
}



void ViamSonus::printDebug(StringBuilder* output) {
  output->concat("ViamSonus 2.0\n");
  #if defined(ADG2128_DEBUG)
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_SWITCH)) {  cp_switch.printDebug(output);  }
  #endif  // ADG2128_DEBUG
  #if defined(DS1881_DEBUG)
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_0)) {   pot0.printDebug(output);       }
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_1)) {   pot1.printDebug(output);       }
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_2)) {   pot2.printDebug(output);       }
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_3)) {   pot3.printDebug(output);       }
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_4)) {   pot4.printDebug(output);       }
    if (_vs_flag(VIAMSONUS_FLAG_FOUND_POT_5)) {   pot5.printDebug(output);       }
  #endif  // DS1881_DEBUG
  for (uint8_t i = 0; i < 8; i++) {
    dumpOutputChannel(i, output);
  }
}


void ViamSonus::dumpOutputChannel(uint8_t chan, StringBuilder* output) {
  if (chan > 7)  {
    output->concat("dumpOutputChannel() was passed an out-of-bounds id.\n");
    return;
  }

  if (outputs[chan].name) {
    output->concatf("    Output: %s\n", outputs[chan].name);
  }
  else {
    output->concatf("    Output channel %u\n", chan);
  }

  if (outputs[chan].i_chans == 0) {
    output->concat("\tPresently unbound\n");
  }
  else {
    output->concat("\tBound to inputs ");
    for (uint8_t i = 0; i < 12; i++) {
      if ((1 << i) & outputs[chan].i_chans) {
        //dumpInputChannel(&inputs[i], output);
        if (nullptr != inputs[i].name) {
          output->concatf("\"%s\", ", inputs[i].name);
        }
        else {
          output->concatf("%u, ", i);
        }
      }
    }
    output->concat("\n");
  }
}


void ViamSonus::dumpInputChannel(CPInputChannel *chan, StringBuilder* output) {
  if (chan == nullptr) {
    output->concat("dumpInputChannel() was passed a NULL InputChannel.\n");
    return;
  }
  if (chan->name) {
    output->concatf("    Input: %s", chan->name);
  }
  else {
    output->concatf("    Input channel %u", chan->i_chan);
  }
  DS1881* pot = _getPotRef(chan->i_chan);
  output->concatf("\tVolume:  %u\n", pot->getValue(chan->i_chan & 1));
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


/*
* Stores everything about the class in the provided buffer in this format...
*   Offset                    | Data
*   --------------------------|------------------------
*   0                         | Serializer version
*   1                         | Flags bits 24-31 (MSB)
*   2                         | Flags bits 16-23
*   3                         | Flags bits 8-15
*   4                         | Flags bits 0-7 (LSB)
*   5                         | ADG2128 data
*   5+ADG2128_SERIALIZE_SIZE  | DS1881E data x6
*   TODO: Input channel data
*   TODO: Output channel data
*
* Returns the number of bytes written to the buffer.
*/
uint32_t ViamSonus::serialize(uint8_t* buf, unsigned int len) {
  uint32_t offset = 0;
  if (len >= VIAMSONUS_SERIALIZE_SIZE) {
    uint32_t f = _flags & VIAMSONUS_FLAG_SERIAL_MASK;
    *(buf + offset++) = VIAMSONUS_SERIALIZE_VERSION;
    *(buf + offset++) = (uint8_t) 0xFF & (f >> 24);
    *(buf + offset++) = (uint8_t) 0xFF & (f >> 16);
    *(buf + offset++) = (uint8_t) 0xFF & (f >> 8);
    *(buf + offset++) = (uint8_t) 0xFF & f;
    if (ADG2128_SERIALIZE_SIZE == cp_switch.serialize((buf + offset), len-offset)) {
      offset += ADG2128_SERIALIZE_SIZE;
      for (uint8_t i = 0; i < 6; i++) {
        if (DS1881_SERIALIZE_SIZE == _getPotRef(i <<1 )->serialize((buf + offset), len-offset)) {
          offset += DS1881_SERIALIZE_SIZE;
        }
      }
    }

    if ((VIAMSONUS_SERIALIZE_SIZE == offset) && (0 < (len - offset))) {
      // If we packed the basics with success, and still have space left in
      //   the buffer, we start looking for definitions of channel groups.
      int c_count = _outputs.size();
      for (int i = 0; i < c_count; i++) {
        VSOGroup* c = _outputs.get(i);
        if (c->serialized_len() > (len - offset)) {
          return 0;
        }
        offset += c->serialize((buf + offset), (len - offset));
      }
      c_count = _inputs.size();
      for (int i = 0; i < c_count; i++) {
        VSIGroup* c = _inputs.get(i);
        if (c->serialized_len() > (len - offset)) {
          return 0;
        }
        offset += c->serialize((buf + offset), (len - offset));
      }
    }
  }
  return offset;
}



int8_t ViamSonus::unserialize(const uint8_t* buf, const unsigned int len) {
  uint32_t offset = 0;
  uint32_t expected_sz = 0xFFFFFFFF;
  int8_t ret = -1;
  if (len >= VIAMSONUS_SERIALIZE_SIZE) {  // The minimum length.
    uint32_t f = (*(buf + 1) << 24) | (*(buf + 2) << 16) | (*(buf + 3) << 8) | *(buf + 4);
    switch (*(buf + offset++)) {
      case VIAMSONUS_SERIALIZE_VERSION:
        expected_sz = VIAMSONUS_SERIALIZE_SIZE;
        offset += 4;  // We'll have already constructed with _ADDR.
        if (0 != cp_switch.unserialize((buf + offset), len-offset)) {
          return -3;
        }
        offset += ADG2128_SERIALIZE_SIZE;  // We'll have already constructed with _ADDR.
        _flags = (_flags & ~VIAMSONUS_FLAG_SERIAL_MASK) | (f & VIAMSONUS_FLAG_SERIAL_MASK);
        for (uint8_t i = 0; i < 6; i++) {
          if (0 != _getPotRef(i << 1)->unserialize((buf + offset), len-offset)) {
            return -4;
          }
          offset += DS1881_SERIALIZE_SIZE;
        }
        ret = (expected_sz == offset) ? 0 : -5;
        break;
      default:  // Unhandled serializer version.
        return -2;
    }
  }
  if ((0 == ret) && (1 < (len - offset))) {
    // If we took apart the basics with success, and still have space left in
    //   the buffer, we start looking for definitions of channel groups.
  }
  return ret;
}


/*******************************************************************************
* VSGroup handling
*******************************************************************************/

VSOGroup* ViamSonus::createOutputGroup(const char* n) {
  VSOGroup* ret = new VSOGroup((const ViamSonus*) this);
  if (nullptr != ret) {
    ret->setName(n);
    _outputs.insert(ret);
  }
  return ret;
}


VSIGroup* ViamSonus::createInputGroup(const char* n) {
  VSIGroup* ret = new VSIGroup((const ViamSonus*) this);
  if (nullptr != ret) {
    ret->setName(n);
    _inputs.insert(ret);
  }
  return ret;
}
