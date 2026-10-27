// Copyright 2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include "flash.h"

#define CONFIG_FLASH_BASE_START 0x00078000
// Work out the page size to use
#ifndef WEAR_LEVELING_LEGACY_EMULATION_PAGE_SIZE
#define WEAR_LEVELING_LEGACY_EMULATION_PAGE_SIZE SECTOR_SIZE
#endif

// Work out how much flash space we have
// #ifndef WEAR_LEVELING_LEGACY_EMULATION_FLASH_SIZE
// #       define WEAR_LEVELING_LEGACY_EMULATION_FLASH_SIZE 0x40000 // in kB
// #endif

// The base location of program memory
#ifndef WEAR_LEVELING_LEGACY_EMULATION_FLASH_BASE
#    define WEAR_LEVELING_LEGACY_EMULATION_FLASH_BASE CONFIG_FLASH_BASE_START
#endif

// The number of pages to use
#ifndef WEAR_LEVELING_LEGACY_EMULATION_PAGE_COUNT
#define WEAR_LEVELING_LEGACY_EMULATION_PAGE_COUNT 1
#endif

// The origin of the emulated eeprom
#ifndef WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS
#   define WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS WEAR_LEVELING_LEGACY_EMULATION_FLASH_BASE // +48k
#endif

// 2-byte writes
#ifndef BACKING_STORE_WRITE_SIZE
#    define BACKING_STORE_WRITE_SIZE 2
#endif

// The amount of space to use for the entire set of emulation
#ifndef WEAR_LEVELING_BACKING_SIZE
#define WEAR_LEVELING_BACKING_SIZE 8192
#endif

// The logical amount of eeprom available
#ifndef WEAR_LEVELING_LOGICAL_SIZE
#define WEAR_LEVELING_LOGICAL_SIZE 4096
#endif
