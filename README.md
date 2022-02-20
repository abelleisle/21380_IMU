# P21380 IMU Firmware

This is the IMU firmware for the RIT MSD Team P21380. 

The goal of this project is to create a cheap, effective, body tracking system
for use in golf-oriented athletes. The body motion of a golf swing can be
tracked by attaching numerous IMUs to the "hotspot" of a golfer's body. This
repo contains the firmware used within each of these IMU devices.

This firmware is built on top of the Zephyr RTOS.

## Table of Contents:
- [Getting Started](#getting-started)
    - [Andy's Quickstart Guide](#andy’s-quickstart-guide)
    - [Installing Zephyr SDK](#installing-zephyr-sdk)
    - [Building & Flashing](#building-%26-flashing)
- [Using IMU Firmware](#how-to-use)
    - [Syncing over BLE](#over-ble)
    - [Syncing over Serial](#shell-debugging)
- [Data Structures](#data-structures)
    - [IMU Data Packet](#imu-packet)
    - [IMU Sensor Data](#imu-data)
- [Bluetooth Specifications](#bluetooth-specifications)

# Getting Started

Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html)
or use Andy's Quickstart Guide for a quick step-by-step guide.

### Andy's Quickstart Guide

1. Make sure Python 3 and PIP are installed on your system. CMake needs to be
   installed as well.
2. Install [West](https://github.com/zephyrproject-rtos/west): `python3 -m pip
   install west`
3. Clone this repo and create a Zephyr workspace: `west init -m
   https://github.com/drumsetmonkey/21380_IMU --mr master MSD_workspace`
    * This will create a folder called MSD\_workspace that the Zephyr RTOS will 
    use for building.
4. Install Zephyr and update dependencies
    * `cd MSD_workspace`
    * `west update`
    * Install Python build dependencies: `python3 -m pip install -r
      zephyr/scripts/requirements.txt`
5. Install the Zephyr SDK (See Below)
6. Enter the code repo directory: `cd 21380_IMU`
    * This is where you will do your work

### Installing Zephyr SDK

The Zephyr SDK must be installed to build this application, I know, it's a pain,
but it makes life easier and provides a lot of tools that are easy to use.

Download the [latest SDK version](https://github.com/zephyrproject-rtos/sdk-ng/releases/latest)

To reduce disk usage on your computer, you can install the ARM only version,
which should be named something like so: `zephyr-toolchain-arm-x.xx.x-linux-x86_64-setup.run`

When it asks you where you'd like to install it, the two default locations are
either `~/zepher-sdk/` or `~/bin/zephyr-sdk`, either of these locations are
preferred, but other locations may be used with extra work if preferred.

### Building & Flashing

The application can be built by running:

```shell
west build -b nrf52832_imu -s firmware -- -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```

The hardware can be flashed with:
```shell
west flash
```
__Note__: This flash script doesn't give any debug or error output, so if
anything goes wrong, you won't know. To avoid this, make sure the J-Link has no
current software connection for debugging, RTT, J-Link server, etc...

Hardware debugging can be done with:
```shell
west attach
```
This will open a GDB session and connect to it.

#### Debug Build:

Note: This is not yet implemented, so it won't do anything

```shell
west build -b $BOARD -s app -- -DOVERLAY_CONFIG=debug.conf
```

# How To Use

The following sections will show how the firmware should be used.

## Pulling IMU Data from Device

Data can be pulled from the device in two ways:
1. Over BLE
2. Over Zephyr's shell interface

### Over BLE

Connect to the Bluetooth device that advertises the IMU sensor service UUID
listed [here](#ble-uuids). IMU data can be obtained by sending commands to the 
sensor attribute listed [here](#ble-uuids).

The process of reading IMU packets from the IMU is as follows:
1. Connect to IMU
    1. Request 4 byte read
    2. This will be how many IMU packets are on the IMU
2. Read the IMU packet
    1. Request 28 byte read
    2. This is a single snapshot of IMU data (described [here](#imu-packet))
3. Repeat step #2 for the number of packets returned during step #1
4. Once all packets are read, disconnect from IMU

### Shell debugging

As of now, the default build contains a shell interface that can be used to run
commands on the device. To stream IMU data onto the NRF52 and get live output
results, run the `stream` command from within the RTT console.

To capture IMU data over a period of time, first run the `start` command, then
after capturing data, run the `stop` command. The shell will then output a list
of all captured data points.

__Note__: This will not play nicely with the `stream` command, so make sure data
is not being streamed while doing this process. Additionally, the current
IMU capture rate (25 captures a second) can only store ~30 seconds of data
before overwriting. If this happens, the shell will warn you after the `stop`
command.

#### Connecting to the shell

The shell is used with the Segger RTT debugging tools. Installation instructions
for these can be found on the SEGGER J-Link site.

Connect to the device using the Segger RTT Viewer. This will allow you to easily
connect to the debugger I/O and send/receive characters to the NRF52.

For the shell to work properly, the Segger RTT viewer must be changed to send
characters on <Enter>. This can be done with: `Input > Sending > Send on Enter.`

__Note__: You must disconnect from the RTT console before uploading code to the
device, otherwise the J-Link upload will fail.


# Data Structures

### IMU Packet

This is a "capture" packet that stores all IMU data from a certain reading. Each
packet contains the system time the data was captured at and data from all 3
axis of both the accelerometer and gyroscope.

* 28 Bytes long
* 224 Bits long

#### Data Layout
| Bytes | Type     | Description |
|-------|----------|-------------|
| 0-3   | uint32\_t| IMU Time    |
| 4-7   | IMU Data | Accel X     |
| 8-11  | IMU Data | Accel Y     |
| 12-15 | IMU Data | Accel Z     |
| 16-19 | IMU Data | Gyro X      |
| 20-23 | IMU Data | Gyro Y      |
| 24-27 | IMU Data | Gyro Z      |

### IMU Data

IMU sensor data. The data read from the IMU by Zephyr is stored in a 64-bit
data structure. For this use case, only 32 of these bits really end up being
used since the IMU won't report huge values. This structure is used to save
space in both memory and for BLE data transmission.

* 4 Bytes long
* 32 Bits long
    - 8 bits of integer precision
    - 24 bits of decimal precision


#### Data Layout
```
| Byte 3 | Byte 2 | Byte 1 | Byte 0 |
|--------|--------|--------|--------|
|IIIIIIII|DDDDDDDD|DDDDDDDD|DDDDDDDD|
```

The integer parts, `I`, are the least significant 8-bits of the IMUs whole number
readings.

The decimal parts, `D`, are a fraction of the whole number in relation to
1,000,000. e.g.: A decimal value of .75 would be represented as 750,000.

# Bluetooth Specifications

### BLE UUIDs

- Sensor Service UUID
  - `1E47B200-F208-4D20-BBC7-DAC31E042204`
- Sensor Attribute UUID
  - `1E47B201-F208-4D20-BBC7-DAC31E042204`

### BLE Advertising Data

- Advertising Information
- 128-bit UUID
  - Sensor Service UUID
- 16-bit UUID
  - CTS service UUID
- 32-bit service data
  - Number of IMU packets on the device

### Scan Response Data
- Device Name
