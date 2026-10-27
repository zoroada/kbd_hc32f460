#pragma once
#include <stdint.h>
#define TIMER_DIFF_16(a,b) ((uint16_t)((a)-(b)))
#define TIMER_DIFF_32(a,b) ((uint32_t)((a)-(b)))
#define TIMER_DIFF_RAW(a,b) TIMER_DIFF_32(a,b)
void timer_init(void);
void timer_clear(void);
uint16_t timer_read(void);
uint32_t timer_read32(void);
uint16_t timer_elapsed(uint16_t last);
uint32_t timer_elapsed32(uint32_t last);
uint32_t timer_read_fast(void);
uint32_t timer_elapsed_fast(uint32_t last);
#define timer_expired(current,expiry) ((int16_t)((current)-(expiry)) >= 0)
#define timer_expired32(current,expiry) ((int32_t)((current)-(expiry)) >= 0)
