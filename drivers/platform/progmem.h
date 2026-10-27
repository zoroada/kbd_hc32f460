#pragma once
#include <stdint.h>
#include <string.h>
#define PROGMEM
#define PSTR(s) (s)
#define pgm_read_byte(p) (*(const uint8_t *)(p))
#define pgm_read_word(p) (*(const uint16_t *)(p))
#define pgm_read_dword(p) (*(const uint32_t *)(p))
#define pgm_read_ptr(p) (*(p))
#define memcpy_P memcpy
#define strcmp_P strcmp
#define strlen_P strlen
#define snprintf_P snprintf
#define vsnprintf_P vsnprintf
