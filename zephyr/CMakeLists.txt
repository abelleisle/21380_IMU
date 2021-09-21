# SPDX-License-Identifier: Apache-2.0

set (BOARD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

list(APPEND ZEPHYR_EXTRA_MODULES
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sensor/icm42688
  )

cmake_minimum_required(VERSION 3.13.1)
#include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(beacon)


target_sources(app PRIVATE
    ../src/main.c
    #    ../src/cts.c
)
