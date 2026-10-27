#include "bootloader.h"
#include "boot_request.h"

/* Request boot mode through Ret-SRAM without erasing Flash. */
void bootloader_jump(void)
{
    Boot_RequestAndReset();
}
