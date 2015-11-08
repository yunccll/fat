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

fat_dentry_t * fat_find_entry_in_sector(uchar * entries, size_t re_size, const char * name){

    size_t idx = index_of(name, '.');
    if(idx == -1){  
        FAT_ERROR("index of . in file_name error! name [%s]\n", name);
        return NULL; 
    }
    //fill the val with name 
    static const char * fmt = "        .   ";
    char buf[16];
    memcpy(buf, fmt, strlen(fmt));
    strncpy(buf, name, idx);
    strncpy(buf+9, name+idx+1, strlen(name)-idx-1); 
    FAT_DEBUG("after fill the val with fn:[%s]\n", buf);


    size_t i;
    for( i = 0; i < re_size; i+= 32)
    {
        fat_dentry_t * ptr =(fat_dentry_t*) (entries + i);
        
        FAT_DEBUG("entry_seq=%lu, fn.ext [%s], sizeof(ptr->name)%lu, sizeof(ptr->ext)%lu\n", i, buf+9, sizeof(ptr->name), sizeof(ptr->ext));
        int ok = strncmp(buf, ptr->name, sizeof(ptr->name)) == 0 && strncmp(buf + 9, ptr->ext, sizeof(ptr->ext)) == 0;
        if(ok) 
            return  ptr;
    }
    return NULL;
}

