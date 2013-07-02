#include "boot.h"

#include "common.h"


void fat_boot_print(fat_boot_t * pb)
{
    assert(pb);
    FAT_PRINT("jmp : %02x %02x %02x\n", pb->jmp[0], pb->jmp[1], pb->jmp[2]);
}

void fat_boot_init(fat_boot_t * pb)
{
    assert(pb);
    pb->jmp[0] = 0x02;
    pb->jmp[1] = 0x42;
    pb->jmp[2] = 0x82;
}
