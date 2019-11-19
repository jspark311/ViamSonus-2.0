#include <Wire.h>
#include <ViamSonus.h>

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



ViamSonusOpts vs_opts(
  0x70,
  13,      // Use this pin for reset
  true,    // Allow many columns to connect to a single row
  false    // Disallow many rows from connecting to a single column
);

ViamSonus vs(&vs_opts);


/*******************************************************************************
* Functions to output things to the console
*******************************************************************************/

void printHelp() {
  StringBuilder output("\nViam Sonus Example ");
  output.concatf("%s\n---< Meta >-------------------------\n", TEST_PROG_VERSION);
  output.concat("?     This output\n");
  output.concat("Q     Reboot\n");

  output.concat("x     Refresh register shadows\n");
  output.concat("I     Reinitialize\n");
  output.concat("i     Viam Sonus info\n");
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
  if (Serial.available()) {
    char c = Serial.read();
    switch (c) {
      case 'I':
        if (VIAMSONUS_ERROR::NO_ERROR == vs.init()) {
          Serial.println("Device initialized.");
        }
        break;
      case 'R':
        if (VIAMSONUS_ERROR::NO_ERROR == vs.reset()) {
          Serial.println("Device reset.");
        }
        break;
      case 'i':
        vs.printDebug();
        break;
    }
  }
}