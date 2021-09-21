# SPDX-License-Identifier: Apache-2.0

if (CONFIG_ICM42688)
  zephyr_include_directories(.)

  zephyr_library()
  zephyr_library_sources(icm42688.c)
  zephyr_library_sources(icm42688_setup.c)
  zephyr_library_sources_ifdef(CONFIG_SPI icm42688_spi.c)
  zephyr_library_sources_ifdef(CONFIG_ICM42688_TRIGGER icm42688_trigger.c)
endif()
