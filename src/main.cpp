#include "ch.h"
#include "hal.h"

// Thread for LED status and wakeup hold
__attribute__((section(".stacks")))
static THD_WORKING_AREA(monitorThreadWA, 256);
static THD_FUNCTION(monitorThread, arg);

int main(void)
{
    halInit();
    chSysInit();

    chThdCreateStatic(
        monitorThreadWA, sizeof(monitorThreadWA),
        LOWPRIO,
        monitorThread, nullptr);

    chThdExit(0);
    return 0;
}

THD_FUNCTION(monitorThread, arg)
{
    (void)arg;
    while(true) {
        palClearLine(LINE_LED_GREEN);
        chThdSleepMilliseconds(1000);
        palClearLine(LINE_LED_RED);
        chThdSleepMilliseconds(1000);
        palSetLine(LINE_LED_GREEN);
        chThdSleepMilliseconds(1000);
        palSetLine(LINE_LED_RED);
        chThdSleepMilliseconds(1000);
    }
}
