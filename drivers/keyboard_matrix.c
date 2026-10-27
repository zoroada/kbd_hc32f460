#include "keyboard.h"  
#include "vial.h" 
#include "keyboard_matrix.h" 
#include "config.h"             
#include "matrix.h"
#include "hc32_ll_gpio.h"
/* matrix state(1:on, 0:off) */
#ifdef SPLIT_ENABLE
static matrix_row_t matrix_shot[MATRIX_ROWS];
#endif
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static uint8_t debouncing = 0;

#define DEBOUNCE 5  /* 消抖时间(ms) */

/* MATRIX_*_PINS are linear pin IDs: port = pin / 16, bit = pin % 16.
 * Port IDs follow the HC32 LL definitions: A, B, C, D, E, H.
 */
static inline uint8_t matrix_gpio_port(uint8_t pin)
{
    return (uint8_t)(pin >> 4U);
}

static inline uint16_t matrix_gpio_mask(uint8_t pin)
{
    return (uint16_t)(1U << (pin & 0x0FU));
}

static inline bool matrix_gpio_read(uint8_t pin)
{
    return GPIO_ReadInputPins(matrix_gpio_port(pin), matrix_gpio_mask(pin)) != PIN_RESET;
}

static inline void matrix_gpio_write(uint8_t pin, bool high)
{
    if (high) {
        GPIO_SetPins(matrix_gpio_port(pin), matrix_gpio_mask(pin));
    } else {
        GPIO_ResetPins(matrix_gpio_port(pin), matrix_gpio_mask(pin));
    }
}

static matrix_row_t read_cols(void);
static void select_row(uint8_t row);
static void unselect_rows(uint8_t row);

/**
 * @brief 初始化键盘阵列
 *
 */
void matrix_init(void)
{
    for (uint_fast8_t i = MATRIX_ROWS; i--;)    
    {
        stc_gpio_init_t gpio;
        (void)GPIO_StructInit(&gpio);
        gpio.u16PinDir   = PIN_DIR_OUT;
        gpio.u16PinState = PIN_STAT_SET; /* idle rows high; selected row is low */
        (void)GPIO_Init(matrix_gpio_port(MATRIX_ROW_PINS[i]), matrix_gpio_mask(MATRIX_ROW_PINS[i]), &gpio);
    }
    for (uint_fast8_t i = MATRIX_COLS; i--;) 
    {
        stc_gpio_init_t gpio;
        (void)GPIO_StructInit(&gpio);
        /* Active-low rows: keep columns high when no key is pressed. */
        gpio.u16PullUp = PIN_PU_ON;
        gpio.u16ExtInt = PIN_EXTINT_OFF;
        (void)GPIO_Init(matrix_gpio_port(MATRIX_COL_PINS[i]), matrix_gpio_mask(MATRIX_COL_PINS[i]), &gpio);
    }
}
/**
 * @brief 释放键盘阵列
 *
 */
void matrix_uninit(void)
{
#ifdef SPLIT_ENABLE
    for (uint_fast8_t i = LEFT_MATRIX_ROWS; i--;)
#else
    for (uint_fast8_t i = MATRIX_ROWS; i--;) 
#endif
    {
        stc_gpio_init_t gpio;
        (void)GPIO_StructInit(&gpio);
        (void)GPIO_Init(matrix_gpio_port(MATRIX_ROW_PINS[i]), matrix_gpio_mask(MATRIX_ROW_PINS[i]), &gpio);
    }
#ifdef SPLIT_ENABLE
    for (uint_fast8_t i = LEFT_MATRIX_COLS; i--;)
#else
    for (uint_fast8_t i = MATRIX_COLS; i--;) 
#endif
    {
        stc_gpio_init_t gpio;
        (void)GPIO_StructInit(&gpio);
        (void)GPIO_Init(matrix_gpio_port(MATRIX_COL_PINS[i]), matrix_gpio_mask(MATRIX_COL_PINS[i]), &gpio);
    }
}
/** read all rows */
static matrix_row_t read_cols(void)
{
    matrix_row_t result = 0;
#ifdef SPLIT_ENABLE
    for (uint_fast8_t c = 0; c < LEFT_MATRIX_COLS; c++) 
#else
    for (uint_fast8_t c = 0; c < MATRIX_COLS; c++) 
#endif
    {
        /* With active-low rows, a pressed key pulls its column low. */
        if (!matrix_gpio_read(MATRIX_COL_PINS[c]))
            result |= 1 << c; 
    }

    return result;
}

static void select_row(uint8_t row)
{
    matrix_gpio_write(MATRIX_ROW_PINS[row], false);
}

static void unselect_rows(uint8_t row)
{
    matrix_gpio_write(MATRIX_ROW_PINS[row], true);
}

uint8_t matrix_scan(void)
{
#ifdef SPLIT_ENABLE
    for (uint8_t i = 0; i < LEFT_MATRIX_ROWS; i++) 
#else
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) 
#endif

    {
        select_row(i);
        DDL_DelayUS(2);
#ifdef HYBRID_MATRIX
        init_cols();
#endif
//不能删除，会导致串键
#ifdef SPLIT_ENABLE
    unsigned int key = irq_lock();
    matrix_row_t cols = read_cols()| matrix_shot[i];
    irq_unlock(key);
#else
    matrix_row_t cols = read_cols();
#endif
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            debouncing = DEBOUNCE;
        }
        unselect_rows(i);
    }
    if (debouncing) {
    if (--debouncing) {
            // no need to delay here manually, because we use the clock.
            // wait_ms(1);
            keyboard_debounce();
        } else {
#ifdef SPLIT_ENABLE
            for (uint8_t i = 0; i < LEFT_MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
#else
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
#endif
        }
    }
    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing)
        return false;
    return true;
}

void matrix_set_row(uint8_t row, uint8_t col)
{
#ifdef SPLIT_ENABLE
    if (row < MATRIX_ROWS) {
        matrix_shot[row] = ((matrix_row_t)col << LEFT_MATRIX_COLS);
    }
#else
    (void)row;
    (void)col;
#endif
}

void matrix_uset_row(uint8_t row, uint8_t col)
{
#ifdef SPLIT_ENABLE
    if (row < MATRIX_ROWS) {
        matrix_shot[row] &= ~((matrix_row_t)col << LEFT_MATRIX_COLS);
    }
#else
    (void)row;
    (void)col;
#endif
}

inline matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}
// #endif

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
#ifdef SPLIT_ENABLE
    for (uint8_t i = 0; i < LEFT_MATRIX_ROWS; i++) 
#else
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) 
#endif
    {
        count += bitpop16(matrix[i]);
    }
    return count;
}
/**
 * @brief 阵列准备睡眠
 * 
 */
void matrix_wakeup_prepare(void)
{
#ifdef SPLIT_ENABLE
    for (uint8_t i = 0; i < LEFT_MATRIX_ROWS; i++)
#else
    for (uint8_t i = 0; i < MATRIX_ROWS; i++)
#endif
    {
        stc_gpio_init_t gpio;
        (void)GPIO_StructInit(&gpio);
        gpio.u16PinDir   = PIN_DIR_OUT;
        gpio.u16PinState = PIN_STAT_RST; /* hold rows active-low while preparing wake */
        (void)GPIO_Init(matrix_gpio_port(MATRIX_ROW_PINS[i]), matrix_gpio_mask(MATRIX_ROW_PINS[i]), &gpio);
    }
#ifdef SPLIT_ENABLE
    for (uint8_t i = 0; i < LEFT_MATRIX_COLS; i++)
#else
    for (uint8_t i = 0; i < MATRIX_COLS; i++)
#endif
    {
        stc_gpio_init_t gpio;
        (void)GPIO_StructInit(&gpio);
        gpio.u16PullUp = PIN_PU_ON;
        gpio.u16ExtInt = PIN_EXTINT_ON;
        (void)GPIO_Init(matrix_gpio_port(MATRIX_COL_PINS[i]), matrix_gpio_mask(MATRIX_COL_PINS[i]), &gpio);
    }
}
