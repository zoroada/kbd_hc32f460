// Copyright 2018-2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "wear_leveling_drivers.h"

/* Vial sizes its dynamic entries while parsing vial.h. Publish capacity before
 * any dependent headers so every translation unit selects the same layout. */
#define TOTAL_EEPROM_BYTE_COUNT (WEAR_LEVELING_LOGICAL_SIZE)

#include "nvm_eeprom_eeconfig_internal.h"
#include "vial.h"

void eeprom_driver_init(void);
void eeprom_driver_erase(void);
void eeprom_read_block(void *buf, const void *addr, size_t len);
void eeprom_write_block(const void *buf, void *addr, size_t len);

uint8_t eeprom_read_byte(const uint8_t *addr);
uint16_t eeprom_read_word(const uint16_t *addr);
uint32_t eeprom_read_dword(const uint32_t *addr);
void eeprom_write_byte(uint8_t *addr, uint8_t data);
void eeprom_write_word(uint16_t *addr, uint16_t value);
void eeprom_write_dword(uint32_t *addr, uint32_t value);
void eeprom_update_block(const void *buf, void *addr, size_t len);
void eeprom_update_byte(uint8_t *addr, uint8_t value);
void eeprom_update_word(uint16_t *addr, uint16_t value);
void eeprom_update_dword(uint32_t *addr, uint32_t value);

void eeprom_driver_format(bool erase);
void eeprom_driver_erase(void);

