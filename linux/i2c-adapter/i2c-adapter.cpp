#include "i2c-adapter.h"

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

using namespace std;


/**************************************************************************
* Constructors / Destructors                                              *
**************************************************************************/

I2CAdapter::I2CAdapter(uint8_t dev_id) {
  bus_online = false;
  bus_in_use = false;
  bus_error  = false;
  debug      = false;
  open_bus_descriptor = -1;

  char *filename = (char *) alloca(24);
  if (sprintf(filename, "/dev/i2c-%d", dev_id) > 0) {
      open_bus_descriptor = open(filename, O_RDWR);
      if (open_bus_descriptor >= 0) {
          bus_online = true;
      }
      else {
          logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to open the i2c bus represented by %s.", filename);
      }
  }
  else {
      logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Somehow we failed to sprintf and build a filename to open i2c bus %d.", dev_id);
  }
}


I2CAdapter::~I2CAdapter() {
    bus_online = false;
    bus_in_use = false;
    if (open_bus_descriptor >= 0) {
        logger.unified_log(__PRETTY_FUNCTION__, LOG_INFO, "Closing the open i2c bus...");
        close(open_bus_descriptor);
    }
}



/**************************************************************************
* Functions that help manage the class and the bus...                     *
**************************************************************************/

bool I2CAdapter::busIdle(void) {
    return (bus_online & !bus_in_use);
}


bool I2CAdapter::busOnline(void) {
    return bus_online;
}


void I2CAdapter::setDebug(bool x) {
	debug = x;
}


/*
* Private function that will switch the addressed i2c device via ioctl. This
*   function is meaningless on anything but a linux system, in which case it
*   will always return true;
* On a linux system, this will only return true if the ioctl call succeeded.
*/
bool I2CAdapter::switch_device(uint8_t nu_addr) {
    bool return_value = false;
    unsigned short timeout = 10000;
    if (nu_addr != last_used_bus_addr) {
        if (!bus_online) {
            // If the bus is either uninitiallized or not idle, decline
            // to switch the device. Return false;
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "i2c bus is not online, so won't switch device. Failing....");
            return return_value;
        }
        else {
            while (bus_in_use && (timeout > 0)) { timeout--; }
            if (bus_in_use) {
                logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "i2c bus was held for too long. Failing....");
                return return_value;
            }

            if (ioctl(open_bus_descriptor, I2C_SLAVE, nu_addr) >= 0) {
                last_used_bus_addr = nu_addr;
                return_value = true;
            }
            else {
                logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to acquire bus access and/or talk to slave at %d.", nu_addr);
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

int I2CAdapter::writeX(uint8_t dev_addr, uint8_t sub_addr, uint16_t byte_count, uint8_t *buf) {
    int return_value = -1;
    uint8_t buffer[byte_count + 1];
    buffer[0] = sub_addr;

    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, byte_count+1) == byte_count+1) {
            bus_error = false;
            return_value = 1;
            if (debug) {
            	char *temp = (char*) alloca(((byte_count+1)*3)+1);
            	memset(temp, 0x00, ((byte_count+1)*3)+1);
            	for (int i = 0; i < (byte_count+1)*3; i++) {
            		sprintf((temp+i*3), "%02x ", buffer[i]);
            	}
            	logger.unified_log(__PRETTY_FUNCTION__, LOG_NOTICE, "Wrote (%s) to %02x", temp, dev_addr);
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


int I2CAdapter::write8(uint8_t dev_addr, uint8_t dat) {
    int return_value = -1;
    uint8_t buffer[1];
    buffer[0] = dat;

    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 1) == 1) {
            bus_error = false;
            return_value = 1;
            if (debug) {
            	char *temp = (char*) alloca(4);
            	memset(temp, 0x00, 4);
           		sprintf(temp, "%02x", buffer[0]);
            	logger.unified_log(__PRETTY_FUNCTION__, LOG_NOTICE, "Wrote (%s) to %02x", temp, dev_addr);
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


int I2CAdapter::write16(uint8_t dev_addr, uint16_t dat) {
    int return_value = -1;
    uint8_t buffer[2];
    buffer[0] = (dat & 0xFF00) >> 8;
    buffer[1] = dat & 0x00FF;

    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 2) == 2) {
            bus_error = false;
            return_value = 2;
            if (debug) {
            	char *temp = (char*) alloca(6);
            	memset(temp, 0x00, 6);
           		sprintf(temp, "%02x %02x", buffer[0], buffer[1]);
            	logger.unified_log(__PRETTY_FUNCTION__, LOG_DEBUG, "Wrote (%s) to %02x", temp, dev_addr);
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


int I2CAdapter::write8(uint8_t dev_addr, uint8_t sub_addr, uint8_t dat) {
    int return_value = -1;
    uint8_t buffer[4];
    buffer[0] = sub_addr;
    buffer[1] = dat;

    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 2) == 2) {
            bus_error = false;
            return_value = 1;
            if (debug) {
            	char *temp = (char*) alloca(6);
            	memset(temp, 0x00, 6);
           		sprintf(temp, "%02x %02x", buffer[0], buffer[1]);
            	logger.unified_log(__PRETTY_FUNCTION__, LOG_DEBUG, "Wrote (%s) to %02x", temp, dev_addr);
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


/*
  This function sends the MSB first.
*/
int I2CAdapter::write16(uint8_t dev_addr, uint8_t sub_addr, uint16_t dat) {
    int return_value = -1;
    uint8_t buffer[4];
    buffer[0] = sub_addr;
    buffer[1] = (dat & 0xFF00) >> 8;
    buffer[2] = dat & 0x00FF;

    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 3) == 3) {
            bus_error = false;
            return_value = 2;
            if (debug) {
            	char *temp = (char*) alloca(9);
            	memset(temp, 0x00, 9);
           		sprintf(temp, "%02x %02x %02x", buffer[0], buffer[1], buffer[2]);
            	logger.unified_log(__PRETTY_FUNCTION__, LOG_DEBUG, "Wrote (%s) to %02x", temp, dev_addr);
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}



uint8_t I2CAdapter::read8(uint8_t dev_addr, uint8_t sub_addr) {
    uint8_t return_value = 0;
    uint8_t buffer[4];
    buffer[0] = sub_addr;
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 1) == 1) {
            if (read(open_bus_descriptor, buffer, 1)) {
                return_value = buffer[0];
                bus_error = false;
            }
            else {
                printf("Failed to read the requested number of bytes from the i2c bus.");
                bus_error = true;
            }
        }
        else {
            printf("Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


uint8_t I2CAdapter::read8(uint8_t dev_addr) {
    uint8_t return_value = 0;
    uint8_t buffer[1];
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 1) == 1) {
            if (read(open_bus_descriptor, buffer, 1)) {
                return_value = buffer[0];
                bus_error = false;
            }
            else {
                printf("Failed to read the requested number of bytes from the i2c bus.");
                bus_error = true;
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


/*
Returns MSB-first.
*/
uint16_t I2CAdapter::read16(uint8_t dev_addr, uint8_t sub_addr) {
    uint16_t return_value = 0;
    uint8_t buffer[2];
    buffer[0] = sub_addr;
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 1) == 1) {
            if (read(open_bus_descriptor, buffer, 2)) {
                return_value = (buffer[0] << 8) + buffer[1];
                bus_error = false;
            }
            else {
                logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to read the requested number of bytes from the i2c bus.");
                bus_error = true;
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


uint16_t I2CAdapter::read16(uint8_t dev_addr, uint16_t sub_addr) {
    uint16_t return_value = 0;
    uint8_t buffer[2];
    buffer[0] = (sub_addr >> 8) & 0xFF;
    buffer[1] = (sub_addr) & 0xFF;
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 2) == 2) {
            if (read(open_bus_descriptor, buffer, 2)) {
                return_value = (buffer[0] << 8) + buffer[1];
                bus_error = false;
            }
            else {
                logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to read the requested number of bytes from the i2c bus.");
                bus_error = true;
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


uint16_t I2CAdapter::read16(uint8_t dev_addr) {
    uint16_t return_value = 0;
    uint8_t buffer[2];
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (read(open_bus_descriptor, buffer, 2)) {
            return_value = (buffer[0] << 8) + buffer[1];
            bus_error = false;
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to read the requested number of bytes from the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}


int I2CAdapter::readX(uint8_t dev_addr, uint8_t sub_addr, uint8_t len, uint8_t *buf) {
    int return_value = -1;
    uint8_t buffer[1];
    buffer[0] = sub_addr;
    if (switch_device(dev_addr)) {
        bus_in_use = true;
        if (write(open_bus_descriptor, buffer, 1) == 1) {
            return_value = read(open_bus_descriptor, buf, len);
            if (return_value == len) {
                bus_error = false;
            }
            else {
                logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to read the requested number of bytes from the i2c bus. Returned %d.", return_value);
                bus_error = true;
                return_value = -1;
            }
        }
        else {
            logger.unified_log(__PRETTY_FUNCTION__, LOG_ERR, "Failed to write a byte (reg address) to the i2c bus.");
            bus_error = true;
        }
        bus_in_use = false;
    }
    return return_value;
}
