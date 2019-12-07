/*
File:   VSGroup.cpp
Author: J. Ian Lindsay
Date:   2019.12.07


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


/*******************************************************************************
* VSGroup
*******************************************************************************/
VSGroup::VSGroup(const ViamSonus* hw, const uint8_t bs) : _VS(hw), _BASE_SER_SIZE(bs) {}


VSGroup::~VSGroup() {
  if (nullptr != _name) {
    free(_name);
    _name = nullptr;
  }
}


/*
* Returns -3 if this group already contains the channel.
*         -2 if the channel is out-of-range.
*         -1 on some other error.
*         Channel position on success.
*/
int8_t VSGroup::addChannel(uint8_t chan) {
  int8_t ret = _add_channel(chan, _next_position());
  if (0 <= ret) {
    _count++;
  }
  return ret;
}


int8_t VSGroup::reverseChannelOrder() {
  int8_t ret = 0;
  uint8_t i = 0;
  while ((0 == ret) & (i < (_count >> 1))) {
    ret = swapChannelPositions(i, _count - i);
    i++;
  }
  if (0 == ret) {
    ret = _apply_to_hardware(false);
  }
  return ret;
}


void VSGroup::printDebug(StringBuilder* output) {
  if (nullptr != _name) {
    output->concatf("    Assigned name:   %s\n", _name);
  }
  if (0 < _count) {
    output->concat("    Bound channels:  { ");
    for (uint8_t i = 0; i < _count; i++) {
      output->concatf("%d %s",
      _channel_at_position(i),
      ((i == (_count-1)) ? " }\n" : ", ")
    );
  }
  }
  output->concatf("    Dirty:           %c\n", _dirty() ? 'y' : 'n');
}


uint32_t VSGroup::serialize(uint8_t* buf, unsigned int len) {
  const uint32_t FULLSIZE = serialized_len();
  if (FULLSIZE <= len) {
    memset(buf, 0, FULLSIZE);
    uint8_t offset = _serialize(buf, len);
    *(buf + offset++) = (uint8_t) 0xFF & (_flags >> 8);
    *(buf + offset++) = (uint8_t) 0xFF & _flags;

    for (uint32_t i = 0; i < (FULLSIZE - offset); i++) {
      *(buf + offset++) = *(_name + i);
    }
    return FULLSIZE;
  }
  return 0;
}


/*
* Returns the full length required to store this channel group.
*/
uint32_t VSGroup::serialized_len() {
  // +1 for group type byte.
  // +2 for flags.
  // +1 for null-term of empty string.
  uint32_t ret   = _BASE_SER_SIZE + VSCG_FIXED_SER_SIZE;
  uint8_t  s_len = 0;
  if (nullptr != _name) {
    while ((VSCG_MAX_NAME_LENGTH > s_len) && (0 != *(_name + s_len))) {
      s_len++;
    }
  }
  return ret + s_len;
}


/*
* No buffer bounds checking is done here. It is assumed it will be done in the
*  constructor.
*/
uint8_t VSGroup::_unserialize_top(const uint8_t* buf, const unsigned int len) {
  uint8_t offset = _unserialize(buf+1, len-1);
  _flags = (*(buf + offset + 1) << 8) + *(buf + offset + 2);
  offset += 3;  // Account for type code and flags.
  uint8_t s_len = 0;
  while ((VSCG_MAX_NAME_LENGTH > s_len) && (0 != *(buf + offset + s_len))) {
    s_len++;
  }
  _name = (char*) malloc(s_len + 1);
  if (nullptr != _name) {
    memcpy(_name, (buf + offset), s_len);
    *(_name + s_len) = 0;
    return offset + s_len + 1;
  }
  return 0;
}



/*******************************************************************************
* VSIGroup
*******************************************************************************/
VSIGroup::VSIGroup(const ViamSonus* hw) : VSGroup(hw, VSIG_FIXED_SER_SIZE) {}

VSIGroup::VSIGroup(const ViamSonus* hw, const uint8_t* buf, const unsigned int len) : VSIGroup(hw) {
  if ((VSCG_FIXED_SER_SIZE + VSIG_FIXED_SER_SIZE) <= len) {
    _unserialize_top(buf, len);
  }
}



int8_t VSIGroup::_add_channel(uint8_t chan, int8_t pos) {
  int8_t ret = -2;
  if ((0 <= pos) && (pos < 12)) {
    uint8_t idx = pos >> 1;
    uint8_t mask = 0x0F << ((pos & 1) << 2);
    _bind_order[idx] = (_bind_order[idx] & ~mask) | (chan << ((pos & 1) << 2));
  }
  return ret;
}

/*
* Returns the first open position, or -1 if there are none.
*/
int8_t VSIGroup::_next_position() {
  for (uint8_t i = 0; i < 6; i++) {
    if (0x0F == ((_bind_order[i] >> 4) & 0x0F)) {   return (2 * i);         }
    if (0x0F == (_bind_order[i] & 0x0F)) {          return (1 + (2 * i));   }
  }
  return -1;
}


int8_t VSIGroup::_channel_at_position(int8_t pos) {
  int8_t ret = -2;
  if (pos < 12) {
    uint8_t temp = (_bind_order[pos >> 1] >> ((pos & 1) ? 4 : 0)) & 0x0F;
    if (temp < 12) {
      // Any number outside the input channel max index results in an error.
      ret = temp;
    }
  }
  return ret;
}


/**
* Swap the channel assignments at the given positions. We do NOT swap their
*   volumes in hardware, so we must also swap their indexed references to not
*   give the impression of a dirty class state if they happen to differ.
* Error-checks parameters, since public fxn.
*/
int8_t VSIGroup::swapChannelPositions(int8_t pos0, int8_t pos1) {
  int8_t ret = -2;
  int8_t chan0 = _channel_at_position(pos0);
  int8_t chan1 = _channel_at_position(pos1);
  if ((chan0 >= 0) && (chan0 < 12)) {
    ret = -1;
    if ((chan1 >= 0) && (chan1 < 12)) {
      uint8_t mask0 = 0x0F << ((pos0 & 1) << 2);
      uint8_t mask1 = 0x0F << ((pos1 & 1) << 2);
      uint8_t nbo0  = _bind_order[pos0 >> 1] & ~mask0;
      uint8_t nbo1  = _bind_order[pos1 >> 1] & ~mask1;
      nbo0 |= chan1 << ((pos0 & 1) << 2);
      nbo1 |= chan0 << ((pos1 & 1) << 2);
      _bind_order_q[pos0 >> 1] = nbo0;
      _bind_order_q[pos1 >> 1] = nbo1;
      uint8_t tmp_vol = _chan_vol_q[pos0];
      _chan_vol_q[pos0] = _chan_vol_q[pos1];
      _chan_vol_q[pos1] = tmp_vol;
      ret = 0;
    }
  }
  return ret;
}


/**
* Returns the volume at the given position in the group.
* Error-checks parameters, since public fxn.
*/
int8_t VSIGroup::volumeAtPosition(int8_t pos) {
  int8_t ret = -1;
  int8_t chan = _channel_at_position(pos);
  if ((chan >= 0) && (chan < 12)) {
    ret = (int8_t) ((ViamSonus*)_VS)->getVolume(chan);
  }
  return ret;
}


/**
* Returns true if we have changes that need to be applied to hardware.
* For an input channel, this means either the channel assignments, their order,
*   or their volumes.
*/
bool VSIGroup::_dirty() {
  for (unsigned int i = 0; i < sizeof(_bind_order); i++) {
    if (_bind_order[i] != _bind_order_q[i]) {
      return true;
    }
  }
  for (unsigned int i = 0; i < _count; i++) {
    if (volumeAtPosition(i) != _chan_vol_q[i]) {
      return true;
    }
  }
  return false;
}


void VSIGroup::printDebug(StringBuilder* output) {
  VSGroup::printDebug(output);
  output->concat("    Channel volumes: { ");
  for (uint8_t i = 0; i < _count; i++) {
    output->concatf("%d %s",
      volumeAtPosition(i),
      ((i == (_count-1)) ? "}\n" : ", ")
    );
  }
}


/**
* Applies any stacked changes to the hardware.
* Since this call will also update any volumes, and since the pots don't
*   have the same defered action capability that the switch does, we order
*   things at this function with awareness as to the order of events that
*   happen in the hardware.
*/
int8_t VSIGroup::_apply_to_hardware(bool defer) {
  int8_t ret = -1;
  return ret;
}


uint8_t VSIGroup::_serialize(uint8_t* buf, unsigned int len) {
  uint8_t offset = 0;
  *(buf + offset++) = 'I';  // Marks this as an input channel.
  for (uint8_t i = 0; i < 6; i++) {
    *(buf + offset++) = _bind_order[i];
  }
  return offset;
}


uint8_t VSIGroup::_unserialize(const uint8_t* buf, const unsigned int len) {
  int8_t ret = 0;
  if (len >= VSIG_FIXED_SER_SIZE) {
    for (uint8_t i = 0; i < 6; i++) {
      _bind_order[i] = *(buf + i);
    }
    ret = VSIG_FIXED_SER_SIZE;
  }
  return ret;
}



/*******************************************************************************
* VSOGroup functions
*******************************************************************************/
VSOGroup::VSOGroup(const ViamSonus* hw) : VSGroup(hw, VSOG_FIXED_SER_SIZE) {}

VSOGroup::VSOGroup(const ViamSonus* hw, const uint8_t* buf, const unsigned int len) : VSOGroup(hw) {
  if ((VSCG_FIXED_SER_SIZE + VSOG_FIXED_SER_SIZE) <= len) {
    _unserialize_top(buf, len);
  }
}


int8_t VSOGroup::_add_channel(uint8_t chan, int8_t pos) {
  int8_t ret = -2;
  if ((0 <= pos) && (pos < 8)) {
    uint32_t mask = 0x0F << (pos << 2);
    _bind_order = (_bind_order & ~mask) | (chan << (pos << 2));
  }
  return ret;
}

/*
* Returns the first open position, or -1 if there are none.
*/
int8_t VSOGroup::_next_position() {
  for (uint8_t i = 0; i < 8; i++) {
    if (0x0F == ((_bind_order >> (4*i)) & 0x0F)) {   return i;     }
  }
  return -1;
}

int8_t VSOGroup::_channel_at_position(int8_t pos) {
  int8_t ret = -1;
  if (pos < 8) {
    uint8_t temp = (_bind_order >> (pos << 2)) & 0x0F;
    if (temp < 8) {
      // Any number outside the output channel max index results in an error.
      ret = temp;
    }
  }
  return ret;
}


/**
* Swap the channel assignments at the given positions.
* Error-checks parameters, since public fxn.
*/
int8_t VSOGroup::swapChannelPositions(int8_t pos0, int8_t pos1) {
  int8_t ret = -2;
  int8_t chan0 = _channel_at_position(pos0);
  int8_t chan1 = _channel_at_position(pos1);
  if ((chan0 >= 0) && (chan0 < 8)) {
    ret = -1;
    if ((chan1 >= 0) && (chan1 < 8)) {
      uint32_t mask0 = 0x0F << (pos0 << 2);
      uint32_t mask1 = 0x0F << (pos1 << 2);
      uint32_t new_bind_order = _bind_order & ~(mask0 | mask1);
      new_bind_order |= chan0 << (pos1 << 2);
      new_bind_order |= chan1 << (pos0 << 2);
      _bind_order_q = new_bind_order;
      ret = 0;
    }
  }
  return ret;
}


/**
* Returns true if we have changes that need to be applied to hardware.
* For an output channel, all that means is channel assignments or their order.
*/
bool VSOGroup::_dirty() {
  return (_bind_order_q != _bind_order);
}


/**
* Applies any stacked changes to the hardware.
* No special treatment of defer is needed here, since we are only dealing with
*   the switch, and not the potentiometers.
*/
int8_t VSOGroup::_apply_to_hardware(bool defer) {
  int8_t ret = -1;
  for (uint8_t i = 0; i < _count; i++) {
  }
  return ret;
}


uint8_t VSOGroup::_serialize(uint8_t* buf, unsigned int len) {
  uint8_t offset = 0;
  *(buf + offset++) = 'O';  // Marks this as an output channel.
  *(buf + offset++) = (uint8_t) 0xFF & (_bind_order >> 24);
  *(buf + offset++) = (uint8_t) 0xFF & (_bind_order >> 16);
  *(buf + offset++) = (uint8_t) 0xFF & (_bind_order >> 8);
  *(buf + offset++) = (uint8_t) 0xFF & _bind_order;
  return offset;
}


uint8_t VSOGroup::_unserialize(const uint8_t* buf, const unsigned int len) {
  int8_t ret = 0;
  if (len >= VSOG_FIXED_SER_SIZE) {
    _bind_order = (*(buf + 0) << 24) | (*(buf + 1) << 16) | (*(buf + 2) << 8) | *(buf + 3);
    ret = VSOG_FIXED_SER_SIZE;
  }
  return ret;
}
