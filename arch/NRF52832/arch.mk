####################################################
# Architecture variables
#
ARCH_MCU = cortex-m4
ARCH_CONFDIR = arch/NRF52832/cfg
ARCH_LDSCRIPT = $(STARTUPLD_CONTRIB)/NRF52832.ld
ARCH_RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
ARCH_FPU = fpv4-sp-d16 # TODO
ARCH_CONTRIB = "true"

####################################################
# Architecture Compilation Files
#

# Startup files
include $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_nrf52.mk

# HAL-OSAL files (optional)
include $(CHIBIOS_CONTRIB)/os/hal/ports/NRF5/NRF52832/platform.mk

# RTOS files (optional)
include $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/mk/port.mk

####################################################
# Board source port
#
ALLCSRC += arch/NRF52832/port/board.c
ALLINC += arch/NRF52832/port/
