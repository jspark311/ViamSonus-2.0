/*
File:   ViamSonusAdvanced.ino
Author: J. Ian Lindsay
Date:   2019.11.30

ViamSonus 2.0 advanced example. This sketch manages the router board as...
  - 5 stereo inputs
  - 2 mono inputs
  - 3 stereo outputs
  - 2 mono outputs with contingent mix capability



This sketch does not demonstate discrete channel manipulation.
See the Basic example for that.

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
const uint8_t VS_CONF_BLOB[] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x70, VS_RESET_PIN, 0x00, 0x00, 0x01, 0x02,
  0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28,
  0x00, 0x00, 0x40, 0x06, 0x01, 0x29, 0x00, 0x00, 0x40, 0x06, 0x01, 0x2A,
  0x00, 0x00, 0x00, 0x06, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x06, 0x01, 0x2C,
  0x00, 0x3F, 0x3F, 0x06, 0x01, 0x2D, 0x00, 0x3F, 0x3F, 0x06
};

// Construct the router with all the config defined. Will be applied on init().
//ViamSonus vs(VS_CONF_BLOB, VIAMSONUS_SERIALIZE_SIZE);
ViamSonus vs(VS_RESET_PIN);


VSIGroup* inputs[7] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
VSOGroup* outputs[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};

VSIGroup* selected_input = nullptr;

/*******************************************************************************
* Functions to output things to the console
*******************************************************************************/

void printHelp() {
  StringBuilder output("\nViam Sonus Example ");
  output.concatf("%s\n---< Meta >-------------------------\n", TEST_PROG_VERSION);
  output.concat("?     This output\n");
  output.concat("i     Viam Sonus info\n");
  output.concat("h     Hardware info\n");
  output.concat("c     Dump channels\n");
  output.concat("g     Dump groups\n");
  output.concat("Q     Reboot\n");

  output.concat("\n---< Low-level >-----------\n");
  output.concat("x     Refresh register shadows\n");
  output.concat("I     Reinitialize\n");
  output.concat("S     Serialize state\n");
  output.concat("R     Reset\n");
  output.concat("A     Apply all groups to hardware\n");

  output.concat("\n---< Input Manipulation >-----------\n");
  output.concat("[0-6] Select input group\n");
  output.concat("[+/-] (In/de)crease volume on selected group\n");
  output.concat("s     Swap channel order in selected group\n");
  output.concat("a     Apply selected group to hardware\n");

  output.concat("!     Bind selected input to Stereo 0\n");
  output.concat("@     Bind selected input to Stereo 1\n");
  output.concat("#     Bind selected input to Stereo 2\n");
  output.concat("$     Bind selected input to Mono 0\n");
  output.concat("%     Bind selected input to Mono 1\n");

  output.concat("\n---< Configuration macros >-----------\n");
  output.concat("P     Setup 8-channel pass-though\n");

  Serial.println((char*) output.string());
}


/*******************************************************************************
* Demonstration fxns
*******************************************************************************/
/*
* You can use unserialize after init to impart a configuration in a single call.
* This one makes the router pass the first 8 inputs to the 8 outputs with no
*   volume attenuation.
*/
int8_t eight_chan_passthrough() {
  const uint8_t VS_CONF_BLOB[] = {
    0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x70, 0x0A, 0x00, 0x00, 0x01, 0x02,
    0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28,
    0x00, 0x00, 0x40, 0x06, 0x01, 0x29, 0x00, 0x00, 0x40, 0x06, 0x01, 0x2A,
    0x00, 0x00, 0x00, 0x06, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x06, 0x01, 0x2C,
    0x00, 0x3F, 0x3F, 0x06, 0x01, 0x2D, 0x00, 0x3F, 0x3F, 0x06
  };
  return vs.unserialize(VS_CONF_BLOB, sizeof(VS_CONF_BLOB));
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
  ViamSonusError err = vs.init(&Wire);
  if (ViamSonusError::NO_ERROR == err) {
    // Add input channel groups.
    // The order of channel addition is important. Two like-cardinality groups
    //   being routed together will have their routes applied by like indicies.
    //   That is, for a stereo signal:
    //     INPUT.0 --> OUTPUT.0
    //     INPUT.1 --> OUTPUT.1
    inputs[0] = vs.createInputGroup("Aux In");
    inputs[0]->addChannel(0);
    inputs[0]->addChannel(1);

    inputs[1] = vs.createInputGroup("Phone speakers");
    inputs[1]->addChannel(2);
    inputs[1]->addChannel(3);

    inputs[2] = vs.createInputGroup("Bluetooth receiver");
    inputs[2]->addChannel(4);
    inputs[2]->addChannel(5);

    inputs[3] = vs.createInputGroup("Computer 0 line out");
    inputs[3]->addChannel(6);
    inputs[3]->addChannel(7);

    inputs[4] = vs.createInputGroup("Computer 1 line out");
    inputs[4]->addChannel(8);
    inputs[4]->addChannel(9);

    inputs[5] = vs.createInputGroup("Microphone");
    inputs[5]->addChannel(10);

    inputs[6] = vs.createInputGroup("Teensy DAC");
    inputs[6]->addChannel(11);

    // Add output channel groups.
    outputs[0] = vs.createOutputGroup("Amplifier");
    outputs[0]->addChannel(0);
    outputs[0]->addChannel(1);

    outputs[1] = vs.createOutputGroup("Headphones");
    outputs[1]->addChannel(2);
    outputs[1]->addChannel(3);

    outputs[2] = vs.createOutputGroup("Stereo 0");
    outputs[2]->addChannel(4);
    outputs[2]->addChannel(5);

    outputs[3] = vs.createOutputGroup("Phone mic");
    outputs[3]->addChannel(6);

    outputs[4] = vs.createOutputGroup("PA amplifier");
    outputs[4]->addChannel(7);
  }
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
      case 'h':
        vs.printHardware(&output);
        break;
      case 'c':
        vs.printChannels(&output);
        break;
      case 'g':
        vs.printGroups(&output);
        break;
      case 'i':
        vs.printDebug(&output);
        output.concat("Presently selected input channel:\n");
        if (nullptr != selected_input) {
          selected_input->printDebug(&output);
        }
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
      case 'P':
        output.concatf("eight_chan_passthrough() returns %d.\n", eight_chan_passthrough());
        break;

      case 'S':   // Save the state into a buffer for later reconstitution.
        {
          uint8_t buffer[512];
          unsigned int written = vs.serialize(buffer, 512);
          if (VIAMSONUS_SERIALIZE_SIZE <= written) {
            StringBuilder::printBuffer(&output, buffer, written, "\t");
            output.concat("\n");
          }
          else {
            output.concatf("serialize() returns %u. Was expecting %u.\n", written, VIAMSONUS_SERIALIZE_SIZE);
          }
        }
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
        selected_input = inputs[(c - 0x30)];
        if (nullptr != selected_input) {
          selected_input->printDebug(&output);
        }
        break;

      case '!':
      case '@':
      case '#':
      case '$':
      case '%':
        if (nullptr != selected_input) {
          uint8_t out_num = 4;
          switch (c) {
            case '!':  out_num--;
            case '@':  out_num--;
            case '#':  out_num--;
            case '$':  out_num--;
            case '%':  break;
          }
          // TODO: Route to output group
          if (ViamSonusError::NO_ERROR != ret) {
            output.concatf("route() returns %s.\n", ViamSonus::errorToStr(ret));
          }
          else {
            output.concatf("Group is now routed to output %u.\n", out_num);
          }
        }
        else {
          output.concat("No input group selected.\n");
        }
        break;

      case '+':
      case '-':
        if (nullptr != selected_input) {
          uint8_t vol = selected_input->getVolume() + ((c == '+') ? -1 : 1);
          int8_t err = selected_input->setVolume(vol);

          if (0 != err) {
            output.concatf("setVolume() returns %d.\n", err);
          }
          else {
            output.concatf("Volume on group is now %u.\n", selected_input->getVolume());
          }
        }
        else {
          output.concat("No input group selected.\n");
        }
        break;

      case 's':
        if (nullptr != selected_input) {
          int8_t err = selected_input->reverseChannelOrder();

          if (0 != err) {
            output.concatf("reverseChannelOrder() returns %d.\n", err);
          }
          else {
            output.concat("Channel order reversed for group.\n");
          }
        }
        else {
          output.concat("No input group selected.\n");
        }
        break;

      case 'Q': (*CPU_RESTART_ADDR = CPU_RESTART_VAL);   break;
      case '?': printHelp();  break;
    }
  }

  if (ViamSonusError::NO_ERROR != vs.poll()) {

  }

  if (output.length() > 0) {
    Serial.print((char*) output.string());
  }
}
