/*
File:   ViamSonusAdvanced.ino
Author: J. Ian Lindsay
Date:   2019.11.30

ViamSonus 2.0 advanced example. This sketch manages the router board as...
  - 5 stereo inputs
  - 2 mono inputs
  - 3 stereo outputs
  - 2 mono outputs with contingent mix capability

Class init is done from a serialized state save.
*/

#include <Wire.h>
#include <ViamSonus.h>
#include <StringBuilder.h>

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004

#define TEST_PROG_VERSION "v1.1"


/*******************************************************************************
* Pin definitions and hardware constants.
*******************************************************************************/
#define SDA_PIN             18
#define SCL_PIN             19
#define VS_RESET_PIN        10  // Optional. Use 255 to inform driver of absence.
#define LED0_PIN            13


/*******************************************************************************
* Globals
*******************************************************************************/
const uint8_t VS_CONF_BLOB = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x70, VS_RESET_PIN, 0x00, 0x00, 0x01, 0x02,
  0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28,
  0x00, 0x00, 0x40, 0x06, 0x01, 0x29, 0x00, 0x00, 0x40, 0x06, 0x01, 0x2A,
  0x00, 0x00, 0x00, 0x06, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x06, 0x01, 0x2C,
  0x00, 0x3F, 0x3F, 0x06, 0x01, 0x2D, 0x00, 0x3F, 0x3F, 0x06
};

// Construct the router with all the config defined. Will be applied on init().
ViamSonus vs(VS_CONF_BLOB, VIAMSONUS_SERIALIZE_SIZE);


/*******************************************************************************
* Functions to output things to the console
*******************************************************************************/

void printHelp() {
  StringBuilder output("\nViam Sonus Example ");
  output.concatf("%s\n---< Meta >-------------------------\n", TEST_PROG_VERSION);
  output.concat("?     This output\n");
  output.concat("i     Viam Sonus info\n");
  output.concat("Q     Reboot\n");

  output.concat("\n---< Low-level >-----------\n");
  output.concat("x     Refresh register shadows\n");
  output.concat("I     Reinitialize\n");
  output.concat("S     Serialize state\n");
  output.concat("R     Reset\n");

  output.concat("\n---< Input Manipulation >-----------\n");
  output.concat("[0-5] Select input group\n");
  output.concat("[+/-] (In/de)crease volume on selected group\n");

  output.concat("!     Bind selected input to Stereo 0\n");
  output.concat("@     Bind selected input to Stereo 1\n");
  output.concat("#     Bind selected input to Stereo 2\n");
  output.concat("$     Bind selected input to Mono 0\n");
  output.concat("%     Bind selected input to Mono 1\n");
  Serial.println((char*) output.string());
}



/*******************************************************************************
* Setup function
*******************************************************************************/

void setup() {
  Serial.begin(115200);
  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();

  // Calling preserveOnDestroy(true) ahead of init() will prevent the class from
  //   resetting the switch on either init() or class teardown. It will also
  //   cause the potentiometers to operate in NVM mode.
  // The switch does not have non-volatile storage, so if the state is to be
  //   rebuilt after a power loss or reset(), the state will need to be
  //   serialized and initialized later with the resuling buffer.
  // vs.preserveOnDestroy(true);
  vs.init(&Wire);
}


/*******************************************************************************
* Main loop
*******************************************************************************/

void loop() {
  StringBuilder output;
  ViamSonusError ret = ViamSonusError::NO_ERROR;
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'i':
        vs.printDebug(&output);
        output.concat("Presently selected input channel:\n");
        vs.dumpInputChannel(in_chan, &output);
        break;
      case 'I':
        ret = vs.init();
        output.concatf("init() returns %s.\n", ViamSonus::errorToStr(ret));
        break;
      case 'x':
        ret = vs.refresh();
        output.concatf("refresh() returns %s.\n", ViamSonus::errorToStr(ret));
        break;
      case 'R':
        ret = vs.reset();
        output.concatf("reset() returns %s.\n", ViamSonus::errorToStr(ret));
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
        in_chan = vs.getInputByRow((c < 'A') ? (c - 0x30) : (c - 0x37));
        vs.dumpInputChannel(in_chan, &output);
        break;

      case '!':
      case '@':
      case '#':
      case '$':
      case '%':
        if (nullptr != in_chan) {
          uint8_t out_num = 4;
          switch (c) {
            case '!':  out_num--;
            case '@':  out_num--;
            case '#':  out_num--;
            case '$':  out_num--;
            case '%':  break;
          }
          ret = vs.route(out_num, in_chan->i_chan);
          if (ViamSonusError::NO_ERROR != ret) {
            output.concatf("route() returns %s.\n", ViamSonus::errorToStr(ret));
          }
          else {
            output.concatf("Channel %u is now routed to output %u.\n", in_chan->i_chan, out_num);
          }
        }
        else {
          output.concat("No input channel selected.\n");
        }
        break;

      case '+':
      case '-':
        if (nullptr != in_chan) {
          uint8_t vol = vs.getVolume(in_chan->i_chan) + ((c == '+') ? -1 : 1);
          ret = vs.setVolume(in_chan->i_chan, vol);
          if (ViamSonusError::NO_ERROR != ret) {
            output.concatf("setVolume() returns %s.\n", ViamSonus::errorToStr(ret));
          }
          else {
            output.concatf("Channel %u volume is now %u.\n", in_chan->i_chan, vs.getVolume(in_chan->i_chan));
          }
        }
        else {
          output.concat("No input channel selected.\n");
        }
        break;

      case 'Q': (*CPU_RESTART_ADDR = CPU_RESTART_VAL);   break;
      case '?': printHelp();  break;
    }
  }

  if (output.length() > 0) {
    Serial.print((char*) output.string());
  }
}
