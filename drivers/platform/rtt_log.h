#pragma once

/* Formatted log output through SEGGER RTT channel 0. */
#ifdef CONSOLE_ENABLE
#    include "SEGGER_RTT.h"
#    define rtt_log(...) SEGGER_RTT_printf(0, __VA_ARGS__)
#else
#    define rtt_log(...) ((void)0)
#endif
