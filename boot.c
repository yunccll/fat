#include "boot.h"

#include "log.h"
#include <assert.h>

void fat_boot_print(fat_boot_t * pb)
{
    assert(pb);
    FAT_DEBUG("test data %d\n", pb->i);
}

void fat_boot_init(fat_boot_t * boot)
{

}
