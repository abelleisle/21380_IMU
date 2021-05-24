####################################################
# Architecture variables
#
ARCH_MCU = cortex-m4
ARCH_CONFDIR = arch/STM32L476-DISCOVERY/cfg
ARCH_LDSCRIPT = $(STARTUPLD)/STM32L476xG.ld
ARCH_RULESPATH = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk
ARCH_FPU = fpv4-sp-d16

####################################################
# Architecture Compilation Files
#

# Startup files
include $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/mk/startup_stm32l4xx.mk

# HAL-OSAL files (optional).
include $(CHIBIOS)/os/hal/ports/STM32/STM32L4xx/platform.mk
include $(CHIBIOS)/os/hal/boards/ST_STM32L476_DISCOVERY/board.mk

# RTOS files (optional)
include $(CHIBIOS)/os/common/ports/ARMv7-M/compilers/GCC/mk/port.mk
