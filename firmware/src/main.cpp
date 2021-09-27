/*
 * Copyright (c) 2020 TDK Invensense
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <device.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/spi.h>
#include <stdio.h>
#include <zephyr.h>

#include <BLE.hpp>

//#define IMU0_NODE DT_ALIAS(invensense,icm42688)
/* #if DT_NODE_HAS_STATUS(IMU0_NODE, okay) */
/* #define IMU0    DT_LABEL(DT_INST(0, icm42688)) */
/* #else */
/* A build error here means your board isn't set up to blink an LED. */
/* #error "Unsupported board: icm42688 devicetree alias is not defined" */
/* #define IMU0 "" */
/* #endif */

#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0 DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0 ""
#define PIN 0
#define FLAGS 0
#endif

static const char *now_str(void) {
  static char buf[16]; /* ...HH:MM:SS.MMM */
  uint32_t now = k_uptime_get_32();
  unsigned int ms = now % MSEC_PER_SEC;
  unsigned int s;
  unsigned int min;
  unsigned int h;

  now /= MSEC_PER_SEC;
  s = now % 60U;
  now /= 60U;
  min = now % 60U;
  now /= 60U;
  h = now;

  snprintf(buf, sizeof(buf), "%u:%02u:%02u.%03u", h, min, s, ms);
  return buf;
}

static int process_icm42688(const struct device *dev) {
  struct sensor_value temperature;
  struct sensor_value accel[3];
  struct sensor_value gyro[3];
  int rc = sensor_sample_fetch(dev);

  if (rc == 0) {
    rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
  }
  if (rc == 0) {
    rc = sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, gyro);
  }
  if (rc == 0) {
    rc = sensor_channel_get(dev, SENSOR_CHAN_DIE_TEMP, &temperature);
  }
  if (rc == 0) {
    printf("[%s]:% g Cel\n"
           "  accel % f % f % f m/s/s\n"
           "  gyro  % f % f % f rad/s\n",
           now_str(), sensor_value_to_double(&temperature),
           sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]),
           sensor_value_to_double(&accel[2]), sensor_value_to_double(&gyro[0]),
           sensor_value_to_double(&gyro[1]), sensor_value_to_double(&gyro[2]));
  } else {
    printf("sample fetch/get failed: %d\n", rc);
  }

  return rc;
}

static struct sensor_trigger data_trigger;
static struct sensor_trigger tap_trigger;
static struct sensor_trigger double_tap_trigger;

static void handle_icm42688_drdy(const struct device *dev,
                                 struct sensor_trigger *trig) {
  int rc = process_icm42688(dev);

  if (rc != 0) {
    printf("cancelling trigger due to failure: %d\n", rc);
    (void)sensor_trigger_set(dev, trig, NULL);
    return;
  }
}

static void handle_icm42688_tap(const struct device *dev,
                                struct sensor_trigger *trig) {
  printf("Tap Detected!\n");
}

static void handle_icm42688_double_tap(const struct device *dev,
                                       struct sensor_trigger *trig) {
  printf("Double Tap detected!\n");
}

void main(void) {
  printk("Starting IMU Demo...\r\n");

  const char *const label = DT_LABEL(DT_INST(0, invensense_icm42688));
  const struct device *icm42688 = device_get_binding(label);
  const struct device *led = device_get_binding(LED0);

  if (led == NULL) {
    printk("Failed to initialize diagnostic LED");
    return;
  }

  int ret = gpio_pin_configure(led, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
  if (ret < 0) {
    printk("Could not configure LED");
    return;
  }

  if (!icm42688) {
    printf("Failed to find sensor %s\n", "ICM42688");
    return;
  }

  tap_trigger = (struct sensor_trigger){
      .type = SENSOR_TRIG_TAP,
      .chan = SENSOR_CHAN_ALL,
  };

  if (sensor_trigger_set(icm42688, &tap_trigger, handle_icm42688_tap) < 0) {
    printf("Cannot configure tap trigger!!!\n");
    return;
  }

  double_tap_trigger = (struct sensor_trigger){
      .type = SENSOR_TRIG_DOUBLE_TAP,
      .chan = SENSOR_CHAN_ALL,
  };

  if (sensor_trigger_set(icm42688, &double_tap_trigger,
                         handle_icm42688_double_tap) < 0) {
    printf("Cannot configure double tap trigger!!!\n");
    return;
  }

  data_trigger = (struct sensor_trigger){
      .type = SENSOR_TRIG_DATA_READY,
      .chan = SENSOR_CHAN_ALL,
  };

  if (sensor_trigger_set(icm42688, &data_trigger, handle_icm42688_drdy) < 0) {
    printf("Cannot configure data trigger!!!\n");
    return;
  }

  printf("Configured for triggered sampling.\n");

  while (0) {
    printk("LED On...\n");
    gpio_pin_set(led, PIN, false);
    k_msleep(500);
    printk("LED Off...\n");
    gpio_pin_set(led, PIN, true);
    k_msleep(500);
  }


  return;
}
