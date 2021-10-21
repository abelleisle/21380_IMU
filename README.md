# P21380 IMU Firmware

## Getting Started

Before getting started, make sure you have a proper Zephyr development
environment. You can follow the official
[Zephyr Getting Started Guide](https://docs.zephyrproject.org/latest/getting_started/index.html)
or use Andy's Quickstart Guide for a quick step by step guide.

### Andy's Quickstart Guide

1. Make sure Python 3 and PIP are installed on your system. CMake needs to be
   installed as well.
2. Install [West](https://github.com/zephyrproject-rtos/west): `python3 -m pip
   install west`
3. Clone this repo and create a Zephyr workspace: `west init -m
   https://github.com/drumsetmonkey/21380_IMU --mr master MSD_workspace`
    * This will create a folder called MSD\_workspace that the Zephyr RTOS will 
    use for building.
4. Install Zephyr and update dependancies
    * `cd MSD_workspace`
    * `west update`
    * Install Python build dependancies: `python3 -m pip install -r
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

## Building & Running

The application can be built by running:

```shell
west build -b nrf52832_imu -s firmware -- -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```

The hardware can be flashed with:
```shell
west flash
```
__Note__: This flash script doesn't give any debug or error output, so if
anything goes wrong, you won't know. To avoid this, make sure the J-Link is not
connected to via, debugging, RTT, J-Link server, etc..

Hardware debugging can be done with:
```shell
west attach
```
this will open a GDB session and connect to it.

### Debug Build:

Note: This is not yet implemented, so it won't do anything

```shell
west build -b $BOARD -s app -- -DOVERLAY_CONFIG=debug.conf
```

## BLE Data

- Sensor Service UUID
  - `1E47B2EE-F208-4D20-BBC7-DAC31E042204`

## Pulling IMU Data from Device

As of now, the default build contains a shell interface that can be used to run
commands on the device. To stream IMU data onto the NRF52 and get live output
results, run the `stream` command from within the rtt console.

To capture IMU data over a period of time, first run the `start` command, then
after capturing data, run the `stop` command. The shell will then output a list
of all captured data points.

__Note__: This will not play nicely with the `stream` command, so make sure data
is not being streamed while doing this process. Additionally, the current
IMU capture rate (25 captures a second) can only store ~30 seconds of data
before overwriting. If this happens, the shell will warn you after the `stop`
command.

### Connecting to the shell
The Shell is used with the Segger RTT debugging tools. Installation instructions
for these can be found on the SEGGER J-Link site.

Connect to the device using the Segger RTT Viewer. This will allow you to easily
connect to the debugger I/O and send/recieve characters to the NRF52.

For the shell to work properly, the Segger RTT viewer must be changed to send
characters on <Enter>. This can be done with: `Input > Sending > Send on Enter.`

__Note__: You must disconnect from the RTT console before uploading code to the
device, overwise the J-Link upload will fail.
