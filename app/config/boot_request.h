#ifndef BOOT_REQUEST_H
#define BOOT_REQUEST_H

#include "hc32_ll.h"

/* Reserved at the top of retention RAM; startup must not initialize these words. */
#define BOOT_DOUBLE_RESET_ADDR  (0x200F0FF0UL)
#define BOOT_DOUBLE_RESET_MAGIC (0x5A1AD5UL)
#define BOOT_DOUBLE_RESET_MS    (500UL)
#define BOOT_REQUEST_ADDR       (0x200F0FF8UL)
#define BOOT_REQUEST_MAGIC      (0x424F4F54UL)

/* A valid request stays set until a complete UF2 update clears it. */
static inline uint32_t Boot_IsRequested(void)
{
    volatile uint32_t *request = (volatile uint32_t *)BOOT_REQUEST_ADDR;
    return (request[0] == BOOT_REQUEST_MAGIC) &&
           (request[1] == ~BOOT_REQUEST_MAGIC);
}

static inline void Boot_SetRequest(void)
{
    volatile uint32_t *request = (volatile uint32_t *)BOOT_REQUEST_ADDR;
    request[0] = BOOT_REQUEST_MAGIC;
    request[1] = ~BOOT_REQUEST_MAGIC;
    __DSB();
}

static inline void Boot_ClearRequest(void)
{
    volatile uint32_t *request = (volatile uint32_t *)BOOT_REQUEST_ADDR;
    request[0] = 0UL;
    request[1] = 0UL;
    __DSB();
}

static inline void Boot_RequestAndReset(void)
{
    __disable_irq();
    *(volatile uint32_t *)BOOT_DOUBLE_RESET_ADDR = 0UL;
    *(volatile uint32_t *)(BOOT_DOUBLE_RESET_ADDR + 4UL) = 0UL;
    Boot_SetRequest();
    NVIC_SystemReset();
}

/* Returns nonzero on the second reset; first reset starts the 500 ms window. */
static inline uint32_t Boot_DoubleResetDetected(void)
{
    volatile uint32_t *state = (volatile uint32_t *)BOOT_DOUBLE_RESET_ADDR;
    uint32_t detected = (state[0] == BOOT_DOUBLE_RESET_MAGIC) &&
                        (state[1] == ~BOOT_DOUBLE_RESET_MAGIC);

    state[0] = detected ? 0UL : BOOT_DOUBLE_RESET_MAGIC;
    state[1] = detected ? 0UL : ~BOOT_DOUBLE_RESET_MAGIC;
    __DSB();
    return detected;
}

static inline void Boot_ClearDoubleReset(void)
{
    *(volatile uint32_t *)BOOT_DOUBLE_RESET_ADDR = 0UL;
    *(volatile uint32_t *)(BOOT_DOUBLE_RESET_ADDR + 4UL) = 0UL;
    __DSB();
}

#endif
