#pragma once
#include "hc32_ll.h"
typedef uint16_t pin_t;
#define NO_PIN 0xFFFFU
#define KEYBOARD_PIN(port,bit) ((pin_t)(((port)<<4)|(bit)))
static inline void setPinInputHigh(pin_t pin) { if(pin==NO_PIN)return; stc_gpio_init_t c; GPIO_StructInit(&c); c.u16PullUp=PIN_PU_ON; GPIO_Init(pin>>4,1U<<(pin&15),&c); }
static inline void setPinInput(pin_t pin) { if(pin==NO_PIN)return; stc_gpio_init_t c; GPIO_StructInit(&c); GPIO_Init(pin>>4,1U<<(pin&15),&c); }
static inline void setPinOutput(pin_t pin) { if(pin==NO_PIN)return; stc_gpio_init_t c; GPIO_StructInit(&c); c.u16PinDir=PIN_DIR_OUT; GPIO_Init(pin>>4,1U<<(pin&15),&c); }
static inline void writePinHigh(pin_t pin) { if(pin!=NO_PIN)GPIO_SetPins(pin>>4,1U<<(pin&15)); }
static inline void writePinLow(pin_t pin) { if(pin!=NO_PIN)GPIO_ResetPins(pin>>4,1U<<(pin&15)); }
static inline bool readPin(pin_t pin) { return pin==NO_PIN || GPIO_ReadInputPins(pin>>4,1U<<(pin&15))!=PIN_RESET; }
static inline void writePin(pin_t p,bool v) { if(v)writePinHigh(p);else writePinLow(p); }
