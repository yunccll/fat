#include "fat_super.h"
#include "base.h"
#include "msdos_fs.h"
#include "mock/access_ok.h"

void fat_sb_free(struct fat_sb * fsb)
{
    free(fsb);
}

static void fat_sb_print(struct fat_sb * fsb)
{
    //TODO:
}
static int fat_sb_read(struct fat_sb  *fsb, const struct fat_boot_sector * bs)
{
    unsigned short val = get_unaligned_le16(bs->sector_size);
    //TODO:
    return 0;
}
struct fat_sb * fat_sb_create(u8 * data, unsigned long size)
{
    struct fat_sb * fsb;
    if(!data){
        return NULL;
    }
    pr_debug("MRB info %x,%x,%x\n", data[0], data[1], data[2]);

    fsb = (struct fat_sb*)calloc(sizeof(struct fat_sb), 1);
    if(fsb && !fat_sb_read(fsb, (const struct fat_boot_sector*)data)){
        fat_sb_print(fsb);
        return fsb;
    }
    fat_sb_free(fsb);
    return NULL;
}
