#include "Wire.h"

// We are being compiled for a linux system.
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <string.h>

using namespace std;


/**************************************************************************
* Constructors / Destructors                                              *
**************************************************************************/

TwoWire::TwoWire(const char* dev_path) {
  bus_online = false;
  bus_in_use = false;
  bus_error  = false;
  debug      = false;
  open_bus_descriptor = -1;

  if (nullptr != dev_path) {
      open_bus_descriptor = open(dev_path, O_RDWR);
      if (open_bus_descriptor >= 0) {
          bus_online = true;
      }
      else {
          printf("Failed to open the i2c bus represented by %s.\n", dev_path);
      }
  }
  else {
      printf("Somehow we failed to sprintf and build a filename to open i2c bus %s.\n", dev_path);
  }
}


TwoWire::~TwoWire() {
    bus_online = false;
    bus_in_use = false;
    if (open_bus_descriptor >= 0) {
        printf("Closing the open i2c bus...\n");
        close(open_bus_descriptor);
    }
}



/**************************************************************************
* Functions that help manage the class and the bus...                     *
**************************************************************************/

bool TwoWire::busIdle(void) {
    return (bus_online & !bus_in_use);
}


bool TwoWire::busOnline(void) {
    return bus_online;
}


void TwoWire::setDebug(bool x) {
  debug = x;
}


int TwoWire::beginTransmission(uint8_t nu_addr) {
  return switch_device(nu_addr) ? 0 : -1;
}


int TwoWire::write(uint8_t byte) {
  return 1;
}

int TwoWire::endTransmission() {
  return 0;
}

int TwoWire::requestFrom(uint8_t addr, unsigned int bytes) {
  return 0;
}

uint8_t TwoWire::receive() {
  return 0;
}


/*
* Private function that will switch the addressed i2c device via ioctl. This
*   function is meaningless on anything but a linux system, in which case it
*   will always return true;
* On a linux system, this will only return true if the ioctl call succeeded.
*/
bool TwoWire::switch_device(uint8_t nu_addr) {
    bool return_value = false;
    unsigned short timeout = 10000;
    if (nu_addr != last_used_bus_addr) {
        if (!bus_online) {
            // If the bus is either uninitiallized or not idle, decline
            // to switch the device. Return false;
            printf("i2c bus is not online, so won't switch device. Failing....\n");
            return return_value;
        }
        else {
            while (bus_in_use && (timeout > 0)) { timeout--; }
            if (bus_in_use) {
                printf("i2c bus was held for too long. Failing....\n");
                return return_value;
            }

            if (ioctl(open_bus_descriptor, I2C_SLAVE, nu_addr) >= 0) {
                last_used_bus_addr = nu_addr;
                return_value = true;
            }
            else {
                printf("Failed to acquire bus access and/or talk to slave at %d.\n", nu_addr);
                bus_error = true;
            }
        }
    }
    else {
        return_value = true;
    }
    return return_value;
}


/**************************************************************************
* Functions that actually result in I/O on the bus...                     *
**************************************************************************/

int TwoWire::writeX(uint8_t dev_addr, uint8_t sub_addr, uint16_t byte_count, uint8_t *buf) {
    int return_value = -1;
    uint8_t buffer[byte_count + 1];
    buffer[0] = sub_addr;

    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (::write(open_bus_descriptor, buffer, byte_count+1) == byte_count+1) {
            bus_error = false;
            return_value = 1;
            if (debug) {
              char *temp = (char*) alloca(((byte_count+1)*3)+1);
              memset(temp, 0x00, ((byte_count+1)*3)+1);
              for (int i = 0; i < (byte_count+1)*3; i++) {
                sprintf((temp+i*3), "%02x ", buffer[i]);
              }
              printf("Wrote (%s) to %02x\n", temp, dev_addr);
            }
        }
        else {
            printf("Failed to write a byte (reg address) to the i2c bus.\n");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}

int TwoWire::readX(uint8_t dev_addr, uint8_t sub_addr, uint8_t len, uint8_t *buf) {
    int return_value = -1;
    uint8_t buffer[1];
    buffer[0] = sub_addr;
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (::write(open_bus_descriptor, buffer, 1) == 1) {
            return_value = ::read(open_bus_descriptor, buf, len);
            if (return_value == len) {
                bus_error = false;
            }
            else {
                printf("Failed to read the requested number of bytes from the i2c bus. Returned %d.\n", return_value);
                bus_error = true;
                return_value = -1;
            }
        }
        else {
            printf("Failed to write a byte (reg address) to the i2c bus.\n");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}



/*
* GPIO manipulation code taken from here and modified.
* TODO: Doesn't bother checking if the pin is I or O.
* https://www.ics.com/blog/how-control-gpio-hardware-c-or-c
*/
void digitalWrite(uint8_t pin, uint8_t val) {
  const int TMP_STR_LEN = 128;
  char temp_pin_buf[TMP_STR_LEN];

  switch (val) {
    case LOW:
    case HIGH:
      break;
    default:
      printf("Unknown value code: %d\n", val);
      exit(1);
  }

  if (pin != 255) {
    memset(temp_pin_buf, 0, TMP_STR_LEN);
    sprintf(temp_pin_buf, "/sys/class/gpio/gpio%u/value", pin);
    int fd = open(temp_pin_buf, O_WRONLY);
    if (fd == -1) {
      printf("Unable to open %s\n", temp_pin_buf);
      exit(1);
    }

    if (::write(fd, (LOW == val) ? "0" : "1", 1) != 1) {
        printf("Error writing to %s.\n", temp_pin_buf);
        exit(1);
    }
    close(fd);
  }
}


/*
* GPIO manipulation code taken from here and modified.
* https://www.ics.com/blog/how-control-gpio-hardware-c-or-c
*/
void pinMode(uint8_t pin, uint8_t io) {
  const int TMP_STR_LEN = 128;
  char temp_pin_buf[TMP_STR_LEN];
  int val_len = 0;
  char* io_string = nullptr;

  switch (io) {
    case OUTPUT:
      io_string = (char*) "out";
      break;
    case INPUT:
      io_string = (char*) "in";
      break;
    default:
      printf("Unknown value code: %d\n", io);
      exit(1);
  }

  if (pin != 255) {
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
      printf("Unable to open /sys/class/gpio/export\n");
      exit(1);
    }
    memset(temp_pin_buf, 0, TMP_STR_LEN);
    sprintf(temp_pin_buf, "%u", pin);
    val_len = strlen(temp_pin_buf);
    if (::write(fd, temp_pin_buf, val_len) != val_len) {
      perror("Error writing to /sys/class/gpio/export");
      exit(1);
    }
    close(fd);

    memset(temp_pin_buf, 0, TMP_STR_LEN);
    sprintf(temp_pin_buf, "/sys/class/gpio/gpio%u/direction", pin);
    val_len = strlen(temp_pin_buf);

    fd = open(temp_pin_buf, O_WRONLY);
    if (fd == -1) {
      printf("Failed to open %s for direction assignment.\n", temp_pin_buf);
      exit(1);
    }

    val_len = strlen(io_string);
    if (::write(fd, io_string, val_len) != val_len) {
      printf("Error writing to %s.\n", temp_pin_buf);
      exit(1);
    }
    close(fd);
  }
}


void delay(unsigned long val) {
  // TODO: Should delay with a signal rather than burn clocks? How accurate
  //   should I expect this to be? usleep() for now.
  // Option 0: Burn the clock.
  //unsigned long start = millis();
  //while ((start + val) > millis()) {
  //}
  // Option 1: signals. Much more complicated. Let's the OS burn the clock.
  // Option 3:
  usleep(val * 1000);
}


/*
* Not provided elsewhere on a linux platform.
*/
unsigned long millis() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000L);
}
