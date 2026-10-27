#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "rtt_log.h"
typedef union debug_config_t { uint8_t raw; struct { bool enable:1; bool matrix:1; bool keyboard:1; bool mouse:1; uint8_t reserved:4; }; } debug_config_t;
extern debug_config_t debug_config;
#define debug_enable debug_config.enable
#define debug_matrix debug_config.matrix
#define debug_keyboard debug_config.keyboard
#define debug_mouse debug_config.mouse
#ifndef NO_DEBUG
#    define dprintf(fmt, ...)                                     \
        do {                                                      \
            if (debug_config.enable) rtt_log(fmt, ##__VA_ARGS__);  \
        } while (0)
#else
#    define dprintf(fmt, ...)
#endif
#define dprint(s) dprintf(s)
#define dprintln(s) dprintf(s "\r\n")
#define dmsg(s) dprintf("%s at %d: %s\n", __FILE__, __LINE__, s)
#define dhex(v) dprintf("%02X", (unsigned int)(v))
#define dhex16(v) dprintf("%04X", (unsigned int)(v))
#define ddec(v) dprintf("%u", (unsigned int)(v))
