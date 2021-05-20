CHIBIOS_VERSION=master
CHIBIOS_CONTRIB_VERSION=chibios-20.3.x

CHIBIOS=os/ChibiOS
CHIBIOS_CONTRIB=os/ChibiOS-Contrib

DEVICE=STM32L476-DISCOVERY

ALLCPPSRC += ./src/main.cpp

include arch/$(DEVICE)/arch.mk

update:
	git subtree pull --prefix os/ChibiOS https://github.com/ChibiOS/ChibiOS $(CHIBIOS_VERSION) --squash
	git subtree pull --prefix os/ChibiOS-Contrib https://github.com/ChibiOS/ChibiOS-Contrib $(CHIBIOS_CONTRIB_VERSION) --squash
