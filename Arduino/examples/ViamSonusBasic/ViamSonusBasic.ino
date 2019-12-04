/*
File:   ViamSonusBasic.ino
Author: J. Ian Lindsay
Date:   2019.11.24

ViamSonus 2.0 basic example. This sketch manages the router board as 12 mono
  inputs into 8 mono outputs with no mix capability.
*/

#include <Wire.h>
#include <ViamSonus.h>
#include <StringBuilder.h>

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004

#define TEST_PROG_VERSION "v1.0"


/*******************************************************************************
* Pin definitions and hardware constants.
*******************************************************************************/
#define SDA_PIN          18
#define SCL_PIN          19
#define VS_RESET_PIN     10  // Optional. Use 255 to inform driver of absence.
#define LED0_PIN         13


/*******************************************************************************
* Globals
*******************************************************************************/
ViamSonus vs(VS_RESET_PIN);          // The actual router hardware.
CPInputChannel* in_chan = nullptr;   // Handle on the current input channel.


/*******************************************************************************
* Functions to output things to the console
*******************************************************************************/

void printHelp() {
  StringBuilder output("\nViam Sonus Example ");
  output.concatf("%s\n---< Meta >-------------------------\n", TEST_PROG_VERSION);
  output.concat("?     This output\n");
  output.concat("i     Viam Sonus info\n");
  output.concat("Q     Reboot\n");

  output.concat("\n---< Input Manipulation >-----------\n");
  output.concat("[0-B] Select input channel\n");
  output.concat("[+/-] (In/de)crease volume on selected channel\n");
  output.concat("x     Refresh register shadows\n");
  output.concat("I     Reinitialize\n");
  output.concat("S     Store device settings in flash\n");
  output.concat("R     Reset\n");

  output.concat("!     Bind selected input to output 0\n");
  output.concat("@     Bind selected input to output 1\n");
  output.concat("#     Bind selected input to output 2\n");
  output.concat("$     Bind selected input to output 3\n");
  output.concat("%     Bind selected input to output 4\n");
  output.concat("^     Bind selected input to output 5\n");
  output.concat("&     Bind selected input to output 6\n");
  output.concat("*     Bind selected input to output 7\n");
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
  // See the advanced usage example for more details.
  // vs.preserveOnDestroy(true);

  vs.init(&Wire);

  // "Input 1", while descriptive, is also not terribly helpful for keeping
  //   track of what is being routed. So we can name them if we like. Making
  //   them reflect the silkscreen lables seems like a good start...
  // This is completely optional. If not done, default names for channels will
  //   be used in any case calling for a label.
  vs.nameInput(0,  "In-L-0");
  vs.nameInput(1,  "In-R-0");
  vs.nameInput(2,  "In-L-1");
  vs.nameInput(3,  "In-R-1");
  vs.nameInput(4,  "In-L-2");
  vs.nameInput(5,  "In-R-2");
  vs.nameInput(6,  "In-L-3");
  vs.nameInput(7,  "In-R-3");
  vs.nameInput(8,  "In-L-4");
  vs.nameInput(9,  "In-R-4");
  vs.nameInput(10, "In-L-5");
  vs.nameInput(11, "In-R-5");
  vs.nameOutput(0, "Out-L-0");
  vs.nameOutput(1, "Out-R-0");
  vs.nameOutput(2, "Out-L-1");
  vs.nameOutput(3, "Out-R-1");
  vs.nameOutput(4, "Out-L-2");
  vs.nameOutput(5, "Out-R-2");
  vs.nameOutput(6, "Out-L-3");
  vs.nameOutput(7, "Out-R-3");
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
      case 'S':   // Save the state into a buffer for later reconstitution.
        {
          uint8_t buffer[VIAMSONUS_SERIALIZE_SIZE];
          unsigned int written = vs.serialize(buffer, VIAMSONUS_SERIALIZE_SIZE);
          if (VIAMSONUS_SERIALIZE_SIZE == written) {
            for (unsigned int i = 0; i < VIAMSONUS_SERIALIZE_SIZE; i++) {
              Serial.print(buffer[i], HEX);
              Serial.print(((i+1) % 16) ? " " : "\n");
            }
            Serial.println();
          }
          else {
            Serial.print("serialize() returns ");
            Serial.print(written);
            Serial.print(". Was expecting ");
            Serial.println(VIAMSONUS_SERIALIZE_SIZE);
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
      case '7':
      case '8':
      case '9':
      case 'A':
      case 'B':
        in_chan = vs.getInputByRow((c < 'A') ? (c - 0x30) : (c - 0x37));
        vs.dumpInputChannel(in_chan, &output);
        break;

      case '!':
      case '@':
      case '#':
      case '$':
      case '%':
      case '^':
      case '&':
      case '*':
        if (nullptr != in_chan) {
          uint8_t out_num = 7;
          switch (c) {
            case '!':  out_num--;
            case '@':  out_num--;
            case '#':  out_num--;
            case '$':  out_num--;
            case '%':  out_num--;
            case '^':  out_num--;
            case '&':  out_num--;
            case '*':  break;
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
