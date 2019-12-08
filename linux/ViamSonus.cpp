/*
File:   ViamSonus.cpp
Author: J. Ian Lindsay
Date:   2014.05.19

Copyright (C) 2014 J. Ian Lindsay
All rights reserved.

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


This is an example program that shows the basic operation of ViamSonus. It is
  intended that this be run on a linux SBC.

*/


#include <ViamSonus.h>
#include <Wire.h>

#define VERSION_STRING "v1.1"

TwoWire* i2c  = nullptr;
ViamSonus* vs = nullptr;


#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include <fcntl.h>
#include <sys/signal.h>
#include <errno.h>
#include <termios.h>
#include <time.h>


/**
* The help function. We use printf() because we are certain there is a user at the other end of STDOUT.
*/
void printUsage() {
	printf("================================================================================\n");
	printf("Bus and channel selection:\n");
	printf("================================================================================\n");
	printf("    --i2c-dev   Specify the i2c device to use.\n");
	printf("-i  --input     input pin (0-11)\n");
	printf("-o  --output    output pin (0-7)\n");
	printf("\n");

	printf("================================================================================\n");
	printf("Options that operate on channels:\n");
	printf("================================================================================\n");
	printf("-r  --route     Route the given input to the given output.\n");
	printf("-u  --unroute   Unroute the given input from the given output(s). If no output\n");
	printf("                 is specified, we will unroute the given input from all outputs.\n");
	printf("-v  --volume    Specify the value of the linear potentiometer (0-255).\n");
	printf("                 If no output is specified, we will adjust all outputs.\n");
	printf("-m  --mute      Mutes a specified output channel. If no output channel is\n");
	printf("                 specified, we will mute all outputs. Note that muting an output\n");
	printf("                 has no effect on the route.\n");
	printf("\n");

	printf("================================================================================\n");
	printf("Meta:\n");
	printf("================================================================================\n");
	printf("-v  --version   Print the version and exit.\n");
	printf("-h  --help      Print this output and exit.\n");
	printf("-s  --status    Read and print the present condition of the PCB.\n");
	printf("-S  --serialize Pack the hardware state into a buffer.\n");
	printf("    --reset     Reset the PCB back to it's power-on state.\n");
	printf("    --enable    Enable a PCB that was previously disabled.\n");
	printf("    --disable   Disable the PCB. Opens all switches and mutes all outputs.\n");
	printf("\n\n");
}



/*******************************************************************************
* Entry-point for a linux build...
********************************************************************************/

int main(int argc, char *argv[]) {
	char operation       = '.';     // No operation.
	uint8_t volume       = 128;
	uint8_t input_chan   = 255;
	uint8_t output_chan  = 255;

	// Parse through all the command line arguments and flags...
	// Please note that the order matters. Put all the most-general matches at the bottom of the loop.
	for (int i = 1; i < argc; i++) {
		if ((strcasestr(argv[i], "--help")) || ((argv[i][0] == '-') && (argv[i][1] == 'h'))) {
			printUsage();
			exit(0);
		}
		else if ((strcasestr(argv[i], "--version")) || ((argv[i][0] == '-') && (argv[i][1] == 'v'))) {
			printf("%s v%s\n\n", argv[0], VERSION_STRING);
			exit(0);
		}
		else if (argc - i >= 2) {    // Compound arguments go in this case block...
			if (strcasestr(argv[i], "--i2c-dev")) {
				i2c = new TwoWire(argv[++i]);          // Fire up the i2c interface...
				i2c->setDebug(true);
			}
			else if (strcasestr(argv[i], "--volume") || ((argv[i][0] == '-') && (argv[i][1] == 'v'))) {
				int temp_vol = atoi(argv[++i]);
				if ((temp_vol > 255) || (temp_vol < 0)) {
					printf("Volume needs to be specified as an integer between 0 and 255.\n");
					exit(0);
				}
				volume = (uint8_t) temp_vol;
				operation = 'v';
			}
			else if (strcasestr(argv[i], "--input") || ((argv[i][0] == '-') && (argv[i][1] == 'i'))) {
				int temp = atoi(argv[++i]);
				if ((temp > 11) || (temp < 0)) {
					printf("Input channel must be between 0 and 11.\n");
					exit(0);
				}
				input_chan = (uint8_t) temp;
			}
			else if (strcasestr(argv[i], "--output") || ((argv[i][0] == '-') && (argv[i][1] == 'o'))) {
				int temp = atoi(argv[++i]);
				if ((temp > 7) || (temp < 0)) {
					printf("Output channel must be between 0 and 7.\n");
					exit(0);
				}
				output_chan = (uint8_t) temp;
			}
			else {
				i++;
			}
		}
		else if (strcasestr(argv[i], "--enable")) {
			operation = 'e';
		}
		else if (strcasestr(argv[i], "--disable")) {
			operation = 'd';
		}
		else if (strcasestr(argv[i], "--reset")) {
			operation = 'x';
		}
		else if (strcasestr(argv[i], "--route") || ((argv[i][0] == '-') && (argv[i][1] == 'r'))) {
			operation = 'r';
		}
		else if (strcasestr(argv[i], "--status") || ((argv[i][0] == '-') && (argv[i][1] == 's'))) {
			operation = 's';
		}
		else if (strcasestr(argv[i], "--serialize") || ((argv[i][0] == '-') && (argv[i][1] == 'S'))) {
			operation = 'S';
		}

		else if (strcasestr(argv[i], "--unroute") || ((argv[i][0] == '-') && (argv[i][1] == 'u'))) {
			operation = 'u';
		}

		else {
			printf("Unhandled argument: %s\n", argv[i]);
			printUsage();
			exit(1);
		}
	}


	if ((nullptr != i2c) && (i2c->busOnline())) {
		vs = new ViamSonus(255);
		// Since this program will do its job and exit immediately (taking the
		//   state of the switch with it), we need to instruct the class to not
		//   disable the hardware when the program exits.
		vs->preserveOnDestroy(true);
		vs->init(i2c);

		ViamSonusError result = ViamSonusError::NO_ERROR;
		switch (operation) {
			case 'r':
				result = vs->route(output_chan, input_chan);
				break;
			case 'u':
				if (input_chan == 255) {
					result = vs->unroute(output_chan);
				}
				else {
					result = vs->unroute(output_chan, input_chan);
				}
				break;
			case 'v':
				if (output_chan == 255) {
					for (int i = 0; i < 8; i++) {
						result = vs->setVolume(i, volume);
					}
				}
				else {
					result = vs->setVolume(output_chan, volume);
				}
				break;
			case '.':
				// No operation selected.
				printf("Nothing to do.\n");
				exit(0);
				break;
			default:
				printf("Unhandled case.\n");
				break;
		}

		switch (result) {
			case ViamSonusError::NO_ERROR:
				printf("Operation completed with success.\n");
				break;
			case ViamSonusError::INPUT_DISPLACED:
				printf("Operation completed with success, but we displaced a previously established route.\n");
				break;
			case ViamSonusError::BAD_COLUMN:
				printf("Error: Output channel is out of range.\n");
				break;
			case ViamSonusError::BAD_ROW:
				printf("Error: Input channel is out of range.\n");
				break;
			case ViamSonusError::UNROUTE_FAILED:
				printf("Error: Failed to unroute the given channels.\n");
				break;
			default:
				printf("Unhandled case: (%s).\n", ViamSonus::errorToStr(result));
				break;
		}
	}
	else {
		printf("You need to supply a valid i2c device.\n");
	}

	exit(0);
}
