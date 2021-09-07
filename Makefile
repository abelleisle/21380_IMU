PLATFORMIO = ${HOME}/.platformio
TOOLS_DIR = $(PLATFORMIO)/packages/tool-jlink/

all:
	pio -f -c vim run

upload:
	pio -f -c vim run -t upload

clean:
	pio -f -c vim run -t clean
	
update:
	pio -f -c vim update

verbose:
	pio -f -c vim run -v

jlink:
	@echo Starting JLink..
	$(TOOLS_DIR)/JLinkExe -device nrf52832_xxAA -if SWD -speed 2000 -autoconnect 1

monitor:
	@echo Starting RTT Monitor...
	$(TOOLS_DIR)/JLinkRTTClient
