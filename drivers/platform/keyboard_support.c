#include "debug.h"
#include "sendchar.h"
#include "suspend.h"
#include "bootloader.h"
#include "hc32_ll.h"
#ifdef CONSOLE_ENABLE
#    include "SEGGER_RTT.h"
#endif

int8_t sendchar(uint8_t c) {
#ifdef CONSOLE_ENABLE
    SEGGER_RTT_PutChar(0, (char)c);
#else
    (void)c;
#endif
    return 0;
}
void suspend_power_down(void) {}
bool suspend_wakeup_condition(void) { return false; }
void suspend_wakeup_init(void) {}
void mcu_reset(void) { NVIC_SystemReset(); }
__attribute__((weak)) void suspend_power_down_user(void) {}
__attribute__((weak)) void suspend_power_down_kb(void) { suspend_power_down_user(); }
__attribute__((weak)) void suspend_wakeup_init_user(void) {}
__attribute__((weak)) void suspend_wakeup_init_kb(void) { suspend_wakeup_init_user(); }
