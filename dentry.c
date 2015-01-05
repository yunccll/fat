#include "dentry.h"

void fat_entry_print(fat_dentry_t * pdentry)
{
    char buf[9] = {0};
    if(pdentry->name[0] != 0x00)
    {
        FAT_DEBUG(" One entry Details ......................  :\n");
        memcpy(buf, pdentry->name, 8);
        FAT_DEBUG("File name                    : [%08s]\n", buf);

        memcpy(buf, pdentry->ext, 3);
        buf[3] = 0;
        FAT_DEBUG("Extension name               : [%03s]\n", buf);
        
        FAT_DEBUG("Attr bits                    : [%02x]\n", pdentry->attr);

        FAT_DEBUG("Case for base & extension    : [%02x]\n", pdentry->lcase);   // not used for fat12
        FAT_DEBUG("Creation time, milliseconds  : [%04x]\n", pdentry->ctime_ms);// not used for fat12
        FAT_DEBUG("Creation time                : [%04x]\n", pdentry->ctime);   // not used for fat12
        FAT_DEBUG("Creation date                : [%04x]\n", pdentry->cdate);   // not used for fat12
        FAT_DEBUG("Last access date             : [%04x]\n", pdentry->adate);   // not used for fat12
        FAT_DEBUG("high 16 bits of first cl.    : [%04x]\n", pdentry->starthi); //for fat32
        FAT_DEBUG("time                         : [%04x],[%d:%d:%d]\n", pdentry->time.val, pdentry->time.detail.hour, pdentry->time.detail.minute, pdentry->time.detail.second*2);
        FAT_DEBUG("date                         : [%04x],[%04d-%02d-%02d]\n", pdentry->date.val, pdentry->date.detail.year+1980, pdentry->date.detail.month, pdentry->date.detail.day);
        FAT_DEBUG("first cluster                : [0x%04x]\n", pdentry->start);
        FAT_DEBUG("file size (in bytes)         : [%08x]\n", pdentry->size);
    }
}
void fat_root_entries_print(uchar * root_entries, size_t  re_size)
{
    assert(root_entries);
    size_t i;
    for(i = 0; i < re_size; i+= 32)
    {
        fat_entry_print((fat_dentry_t*)(root_entries + i));
    }
}

