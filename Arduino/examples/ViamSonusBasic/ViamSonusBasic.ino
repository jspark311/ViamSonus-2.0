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
#define SDA_PIN             18
#define SCL_PIN             19
#define VS_RESET_PIN        10
#define LED0_PIN            13


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
  vs.init();
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
        if (ViamSonusError::NO_ERROR == ret) {
          output.concatf("init() returns %s.\n", ViamSonus::errorToStr(ret));
        }
        break;
      case 'R':
        ret = vs.reset();
        if (ViamSonusError::NO_ERROR == ret) {
          output.concatf("reset() returns %s.\n", ViamSonus::errorToStr(ret));
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

      case '+':
      case '-':
        if (nullptr != in_chan) {
          ret = vs.setVolume(in_chan->i_chan, vs.getVolume(in_chan->i_chan) + (c == '+') ? 1 : -1);
          if (ViamSonusError::NO_ERROR == ret) {
            output.concatf("setVolume() returns %s.\n", ViamSonus::errorToStr(ret));
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
