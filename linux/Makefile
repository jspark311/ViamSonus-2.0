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
CPP_STANDARD = gnu++11

CXX       = gcc
SZ        = size
CXXFLAGS  = -Wall -fsingle-precision-constant -Wdouble-promotion -D_GNU_SOURCE
CXXFLAGS += -Wl,--gc-sections -O2 -fno-rtti -fno-exceptions -fpic
LIBS	    = -lstdc++

export BUILD_ROOT    = $(shell pwd)
export OUTPUT_PATH   = $(BUILD_ROOT)/build/


###########################################################################
# Source files, includes, and linker directives...
###########################################################################
INCLUDES    = -I$(BUILD_ROOT)/.
INCLUDES   += -I$(BUILD_ROOT)/CppPotpourri/StringBuilder
INCLUDES   += -I$(BUILD_ROOT)/CppPotpourri/DataStructures
INCLUDES   += -I$(BUILD_ROOT)/ADG2128/Arduino/src/
INCLUDES   += -I$(BUILD_ROOT)/DS1881/Arduino/src/
INCLUDES   += -I$(BUILD_ROOT)/i2c-adapter
INCLUDES   += -I$(BUILD_ROOT)/../Arduino/src

SRCS  = $(BUILD_ROOT)/ViamSonus.cpp
SRCS += $(BUILD_ROOT)/../Arduino/src/ViamSonus.cpp
SRCS += $(BUILD_ROOT)/../Arduino/src/VSGroup.cpp
SRCS += $(BUILD_ROOT)/../Arduino/src/VSIGroup.cpp
SRCS += $(BUILD_ROOT)/../Arduino/src/VSOGroup.cpp
SRCS += $(BUILD_ROOT)/i2c-adapter/i2c-adapter.cpp
SRCS += $(BUILD_ROOT)/CppPotpourri/StringBuilder/StringBuilder.cpp
SRCS += $(BUILD_ROOT)/ADG2128/Arduino/src/ADG2128.cpp
SRCS += $(BUILD_ROOT)/DS1881/Arduino/src/DS1881.cpp

CXXFLAGS += $(INCLUDES)
CXXFLAGS += -DDS1881_DEBUG=1 -DADG2128_DEBUG=1


OBJS  = $(SRCS:.cpp=.o)

###########################################################################
# Rules for building the host-side follow...
###########################################################################

.PHONY: all

all:	 $(OBJS) builddir
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_NAME) $(OBJS) $(LIBS)
	$(SZ) $(PROGRAM_NAME)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(MANUVR_OPTIONS) -std=$(CPP_STANDARD) -nostdlib -c $< -o $@

install: all
	cp $(PROGRAM_NAME) /usr/bin/

uninstall:
	rm /usr/bin/$(PROGRAM_NAME)

builddir:
	mkdir -p $(OUTPUT_PATH)

clean:
	rm -rf $(OUTPUT_PATH)
	rm -f $(PROGRAM_NAME) $(OBJS)
