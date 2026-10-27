// Copyright 2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#include <stdbool.h>
#include "timer.h"
#include "wear_leveling.h"
#include "wear_leveling_internal.h"
#include "wear_leveling_legacy_config.h"


bool backing_store_init(void) {
    //printf("Backing store: Init\n");
    return true;
}

bool backing_store_unlock(void) {
    //printf("Backing store: Unlock\n");
    // FLASH_Unlock();
    return true;
}

bool backing_store_erase(void) {
#ifdef WEAR_LEVELING_DEBUG_OUTPUT
    uint32_t start = timer_read32();
#endif

    bool         ret = true;
    FLASH_Status status;
    for (int i = 0; i < (WEAR_LEVELING_LEGACY_EMULATION_PAGE_COUNT); ++i) {
        status = FLASH_ErasePage(WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS + (i * (WEAR_LEVELING_LEGACY_EMULATION_PAGE_SIZE)));
        if (status != FLASH_COMPLETE) {
            ret = false;
        }
    }

    //printf("Backing store: Backing store erase took %ldms to complete\n", ((long)(timer_read32() - start)));
    return ret;
}

bool backing_store_write(uint32_t address, backing_store_int_t value) {
    uint32_t offset = ((WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS) + address);
    //printf("Backing store: Write ");
    wl_dump(offset, &value, sizeof(backing_store_int_t));
    return FLASH_ProgramHalfWord(offset, ~value) == FLASH_COMPLETE;

}

bool backing_store_lock(void) {
    //printf("Backing store: Lock  \n");
    // FLASH_Lock();
    return true;
}

bool backing_store_read(uint32_t address, backing_store_int_t* value) {
    uint32_t             offset = ((WEAR_LEVELING_LEGACY_EMULATION_BASE_PAGE_ADDRESS) + address);
    *value                      = ~(FLASH_read_data(offset));
    // wl_dump(offset, loc, sizeof(backing_store_int_t));
    return true;
}
