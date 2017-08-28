#include "boot.h"

#include "device.h"


void fat_boot_print(fat_boot_t * pb)
{
    assert(pb);

    char buf[32];
    PRINT_OUT("jmp              : 0x%02x 0x%02x 0x%02x\n", pb->jmp[0], pb->jmp[1], pb->jmp[2]);
    memcpy(buf,pb->sys_id, 8);
    buf[8] = '\0';
    PRINT_OUT("system id        : %s\n", buf);
    PRINT_OUT("bytes per sec    : %d\n", two_bytes_to_int(pb->sector_size));
    PRINT_OUT("secs per cluster : %d\n", pb->cluster_size);
    PRINT_OUT("number of FATs   : %d\n", pb->fats);
    PRINT_OUT("root dir entries : %d\n", two_bytes_to_int(pb->dir_entries));
    PRINT_OUT("number of secs   : %d\n", two_bytes_to_int(pb->sectors));
    PRINT_OUT("media            : 0x%02x\n", pb->media);
    PRINT_OUT("secs per FAT     : %d\n", pb->fat_length);
    PRINT_OUT("secs per track   : %d\n", pb->secs_track);
    PRINT_OUT("heads            : %d\n", pb->heads);
    PRINT_OUT("hidden secs      : %d\n", pb->hidden);
    // PRINT_OUT("total secs       : %d\n", pb->total_sect);

    PRINT_OUT("\nmsdos volume info: \n");
    PRINT_OUT("bios drive number: 0x%02x\n", pb->vi.drive_number);
    PRINT_OUT("reserved         : 0x%02x\n", pb->vi.RESERVED);
    PRINT_OUT("extent boot sign : 0x%02x\n", pb->vi.ext_boot_sign);
    PRINT_OUT("volume id number : 0x%02x 0x%02x 0x%02x 0x%02x \n", pb->vi.volume_id[0], pb->vi.volume_id[1], pb->vi.volume_id[2], pb->vi.volume_id[3]);

    memcpy(buf,pb->vi.volume_label, 11);
    buf[11] = '\0';
    PRINT_OUT("volume label     : %s\n", buf);

    memcpy(buf,pb->vi.fs_type, 8);
    buf[8] = '\0';
    PRINT_OUT("fs type          : %s#\n", buf);
    PRINT_OUT("boot sign        : 0x%02x 0x%02x\n", (uchar)pb->boot_sign, (uchar)(pb->boot_sign >> 8));
}

void fat_boot_format12(fat_boot_t * pb)
{
    assert(pb);
    memset(pb, 0, sizeof(fat_boot_t));

    pb->jmp[0] = 0xeb;
    pb->jmp[1] = 0x3c;
    pb->jmp[2] = 0x90;

    *((short *)pb->sector_size) = 512;
    pb->cluster_size   = 1;
    pb->fats = 2;

    *((short * )pb->dir_entries) = 224;
    *((short * )pb->sectors) = 2880;
    pb->media = 0xf0;
    pb->fat_length = 9;
    pb->secs_track =  18;
    pb->heads = 2;

    pb->vi.ext_boot_sign = 0x29;
    *((unsigned int*)pb->vi.volume_id) = 0x73bde084;
    memcpy(pb->vi.fs_type, "FAT12   ", 8);

    pb->boot_sign = 0xAA55;
}

void fat_boot_init(fat_boot_t * pb)
{
    assert(pb);
    memset(pb, 0, sizeof(fat_boot_t));
}

int fat_boot_read(fat_boot_t * pb, const char * file, fat_offset_t offset)
{
    assert(pb && file);
    if(file == NULL){
        FAT_ERROR("file name is NULL\n");
        return -1;
    }

    DECLARE_DEVICE_R(fdev, file);
    int ret =fat_dev_read(&fdev, 0, (uchar*)pb, sizeof(*pb));
    if(ret < 0){
        FAT_ERROR("device read failed!\n");
        return -1;
    }
    fat_dev_destroy(&fdev);
    return 0;
}
int fat_boot_write_to_dev(fat_boot_t * pb, fat_dev_t * pdev, fat_offset_t offset){
    int ret =fat_dev_write(pdev, offset, (const uchar*)pb, sizeof(*pb));
    if(ret < 0 ){
        FAT_ERROR("device write failed!\n");
    }
    return ret;
}
int fat_boot_write(fat_boot_t * pb, const char * file, fat_offset_t offset)
{
    assert(pb && file);
    if(file == NULL){
        FAT_ERROR("file name is NULL\n");
        return -1;
    }

    DECLARE_DEVICE_W(fdev, file);
    int ret = fat_boot_write_to_dev(pb, &fdev, offset);
    fat_dev_destroy(&fdev);
    return ret;
}

