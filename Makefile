###########################################################################
# Makefile for ViamSonus-2.0
# Author: J. Ian Lindsay
###########################################################################
SHELL          = /bin/sh
WHO_I_AM       = $(shell whoami)
HOME_DIRECTORY = /home/$(WHO_I_AM)

PROGRAM_NAME   = viamsonus


###########################################################################
# Variables for the host compilation...
###########################################################################
CC       = gcc
CFLAGS   = -Wall
CXXFLAGS = -std=gnu++11
LIBS	   = -lstdc++

export BUILD_ROOT    = $(shell pwd)
export OUTPUT_PATH   = $(BUILD_ROOT)/build/

SRCS  = ViamSonus/*.cpp i2c-adapter/*.cpp linux/*.cpp ADG2128/*.cpp

###########################################################################
# Rules for building the host-side follow...
###########################################################################
default:	viamsonus

debug:		$(PROGRAM_NAME).o
	$(CC) $(CFLAGS) -ggdb -g -o $(PROGRAM_NAME) $(PROGRAM_NAME).o $(LIBS)

builddir:
	mkdir -p $(OUTPUT_PATH)

$(PROGRAM_NAME):	$(PROGRAM_NAME).o
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) *.o $(LIBS)

$(PROGRAM_NAME).o:	builddir ViamSonus.cpp
	$(CC) $(CXXFLAGS) $(CFLAGS) -c ViamSonus.cpp $(SOURCE_FILE_LIST) -fno-exceptions

install: $(PROGRAM_NAME)
	cp $(PROGRAM_NAME) /usr/bin/

uninstall: $(PROGRAM_NAME)
	rm /usr/bin/$(PROGRAM_NAME)

clean:
	rm -rf $(OUTPUT_PATH)
	rm -f $(PROGRAM_NAME) *.o *~
