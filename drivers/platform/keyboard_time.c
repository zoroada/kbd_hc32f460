#include "hc32_ll.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timer.h"
#include "wait.h"
static uint32_t timer_offset;
static uint32_t tick_milliseconds(void)
{
    TickType_t ticks=__get_IPSR()?xTaskGetTickCountFromISR():xTaskGetTickCount();
    return ticks*(1000U/configTICK_RATE_HZ);
}
void timer_init(void) { timer_offset=tick_milliseconds(); }
void timer_clear(void) { timer_offset=tick_milliseconds(); }
uint32_t timer_read32(void) { return tick_milliseconds()-timer_offset; }
uint16_t timer_read(void) { return (uint16_t)timer_read32(); }
uint16_t timer_elapsed(uint16_t t) { return (uint16_t)(timer_read()-t); }
uint32_t timer_elapsed32(uint32_t t) { return timer_read32()-t; }
uint32_t timer_read_fast(void) { return timer_read32(); }
uint32_t timer_elapsed_fast(uint32_t t) { return timer_elapsed32(t); }
void wait_ms(uint16_t ms) { DDL_DelayMS(ms); }
void wait_us(uint16_t us) { DDL_DelayUS(us); }
