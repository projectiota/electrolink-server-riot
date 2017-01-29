# Electrolink for RIOT
[![License](https://img.shields.io/badge/license-Apache%20v2.0-blue.svg)](LICENSE)

```Electrolink for RIOS``` is a [![RIOT RTOS](https://riot-os.org/images/logo-menu.png)](https://riot-os.org/#home) application that is implementing [Electrolink protocol specification](https://github.com/projectiota/electrolink/blob/master/electrolink-protocol.md)

# Supported boards
The firmware is currently in a development phase and only supports [WeIO](http://we-io.net/hardware/).
Once the first stable version will be released, efforts will be puts on supporting more boards.

# Usage

## Preparing the environment

First, clone the repository

```
git clone https://github.com/ks156/weio_firmware
```

Clone RIOT-OS. By default, RIOS-OS is cloned on the top level. If RIOT-OS is present elsewhere on your system, you'll have to set ```RIOTBASE``` in the Makefile to point to the correct PATH.

```
git clone https://github.com/RIOT-OS/RIOT
```

WeIO uses a Cortex-M0 MCU. To build the firmware for this architecture, you have to install a toolchain. The easiest way is to download and install the [GNU ARM Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).
Don't forget to add the toolchain binaries in your path.

## Building the application

Basically, the application can be build with the ```make``` command.

At the end of the build process, the binary file is located in ```bin/weio/WeIO_firmware.bin```

NXP LPC chips have an embedded USB bootloader that require a binary with a CRC after the vector table. This CRC can be automatically added in the binary file with the command ```make crc```

## Flashing WeIO

Before flashing the binary on WeIO, USDaemon must be stopped.

```/etc/init.d/usd stop```

If USDaemon is running, the flashing process will fails.

Transfer ```WeIO_firmware.bin``` and the script ```flash_lpc_fw.py``` located inside ```tools/lpc_flasher``` to WeIO. The binary file must be located in the ```/root``` directory.

Run the script with the following command :
```
python flash_lpc_fw.py
```

## Testing

The script ```test_from_weio.py``` is a basic script that will blink a LED and read the status of GPIO2.
