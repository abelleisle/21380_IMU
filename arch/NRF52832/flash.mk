# Flash
JLINK_DEVICE    = nrf52
JLINK_PRE_FLASH = w4 4001e504 1
#JLINK_ERASE_ALL = w4 4001e504 2\nw4 4001e50c 1\nsleep 100
JLINK_PIN_RESET = w4 40000544 1

OHEX = $(BUILDDIR)/$(PROJECT).hex
OELF = $(BUILDDIR)/$(PROJECT).elf
OBIN = $(BUILDDIR)/$(PROJECT).bin

####################################################
# Board targets
#
include $(CHIBIOS_CONTRIB)/os/various/jlink.mk
include $(CHIBIOS_CONTRIB)/os/various/gdb.mk

pin-reset: jlink-pin-reset
flash: all jlink-flash
debug: gdb-debug
erase-all: jlink-erase-all
debug-server: jlink-debug-server
