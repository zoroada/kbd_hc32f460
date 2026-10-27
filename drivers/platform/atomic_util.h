#pragma once
#include "hc32_ll.h"
static inline uint32_t keyboard_irq_save(void) { uint32_t p=__get_PRIMASK(); __disable_irq(); return p; }
static inline void keyboard_irq_restore(uint32_t *p) { __set_PRIMASK(*p); }
#define ATOMIC_RESTORESTATE 0
#define ATOMIC_BLOCK(type) for (uint32_t _irq __attribute__((cleanup(keyboard_irq_restore)))=keyboard_irq_save(), _once=1; _once; _once=0)
