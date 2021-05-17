# Included Makefiles
include $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_nrf52.mk
include $(CHIBIOS_CONTRIB)/os/hal/ports/NRF5/NRF52832/platform.mk
# Linker files
LDSCRIPT=$(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/ld/NRF52832.ld
