#!/usr/bin/env python

# Copyright (c) 2021 Martin Roesch
# SPDX-License-Identifier: Apache-2.0

import logging
import struct
import time
import argparse

from bluez import Manager

# Setup logging
logging.basicConfig(level=logging.INFO) # Set to DEBUG for debug logs of bluez module
log = logging.getLogger('Throughput')
log.setLevel(logging.INFO) # Set to DEBUG for debug logs of this script

# Test parameters
configInterval = 100 # Notification interval in milliseconds
configDataLen = 200 # Notification data size in bytes
numDataNotifications = 10 # Number of notifications to receive from the data characteristic

# Command line arguments
parser = argparse.ArgumentParser(description='Throughput test.')
parser.add_argument('-i', '--interval', type=int, help = 'Notification interval in milliseconds')
parser.add_argument('-l', '--length', type=int, help = 'Notification data size in bytes')
parser.add_argument('-n', '--num', type=int, help = 'Number of notifications to receive')
args = parser.parse_args()
if args.interval:
    configInterval = args.interval
if args.length:
    configDataLen = args.length
if args.num:
    numDataNotifications = args.num

try:
    mgr = Manager();
    a = mgr.get_adapter('hci0')
    print(f'Using {a}')
    
    serviceUUID = '1e47b200-f208-4d20-bbc7-dac31e042204'
    print(f'Discover device hosting the throughput service {serviceUUID} for 10 seconds.')
    def check(device):
        uuids = device.UUIDs
        log.debug(f'Check UUIDs: {uuids}')
        return serviceUUID in uuids
    device = a.discover_device(check)
    if device:
        print('Found.')
        print(f'Connect to {device}')
        device.connect()
        print('Done.')
        
        services = device.get_gattservices();
        log.debug(f'GATT services: {services}')
        print(f'Check for throughput service {serviceUUID}')
        if serviceUUID in services.keys():
            service = services[serviceUUID]
            print('Found.')
            
            chars = service.get_gattcharacteristics()
            log.debug(f'GATT characteristics: {chars!r}')
            IMUCharUUID = '1e47b201-f208-4d20-bbc7-dac31e042204'
            IMUChar = chars[IMUCharUUID]
            if IMUChar:
                print(f'Read IMU characteristic {IMUCharUUID} parameters:')
                length = IMUChar.ReadValue()

                print(f'Number of IMU items: {length}')
            
#            if dataChar:
#                with dataChar.fd_notify() as q:
#                    print(f'Receive {numDataNotifications} notifications from data characteristic {dataCharUUID}')
#                    startTime = time.time()
#                    lastNotificationTime = startTime
#                    notificationCount = 0
#                    dataSize = 0
#                    stats = {
#                        "dt_min": 1.0,
#                        "dt_max": 0.0
#                        }
#                    for i in range(numDataNotifications):
#                        n = q.get()
#                        notificationTime = time.time()
#                        notificationCount += 1
#                        dataSize += len(n)
#                        dt = notificationTime - lastNotificationTime
#                        if i > 1:
#                            if dt < stats["dt_min"]:
#                                stats["dt_min"] = dt
#                            if dt > stats["dt_max"]:
#                                stats["dt_max"] = dt
#                        print(f'Notification {i+1:4}: {len(n):3} bytes, timestamp: {(notificationTime - startTime):{7}.{3}} s, '
#                              f'dt: {(dt):{7}.{3}} s')
#                        lastNotificationTime = notificationTime
#                    endTime = time.time()
#                    throughput = dataSize * 8 / (endTime - startTime) / 1000
#                    print(f'Summary: Received {dataSize} bytes in {notificationCount} notificattions '
#                          f'during {endTime - startTime:.{3}} seconds: {throughput:.3f} kbits/sec.')
#                    print(f'min dt: {(stats["dt_min"]*1000):{7}.{3}} ms, max dt: {(stats["dt_max"]*1000):{7}.{3}} ms.')
        else:
            print(f'Throughput service {serviceUUID} not found on {device}')
        print(f'Disconnect {device}')
        device.disconnect()
        print('Done.')
    else:
        print('Not found.')
except BaseException as e:
    print(f'Caught exception: {e}')
    raise e
print('Exit')
