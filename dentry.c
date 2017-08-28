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


#define MAX_FAT12_FILE_NAME_LEN 8
#define MAX_FAT12_FILE_EXT_LEN 3
#define FILE_NAME_EXT_SEP_CHAR  '.'


static void _format_file_name(char * fmt_buf, const int len, const char * fname, const int fname_len){
    size_t i = 0;
    //----2.1 prepare buffer with init with space
    for( i = 0; i < len-1; ++i) fmt_buf[i] = (char)0x20;
    fmt_buf[i] = '\0';
    //----2.2 set file name
    for( i = 0; i < fname_len; ++i) fmt_buf[i] = fname[i];
    //----2.3 set file ext
    fmt_buf[MAX_FAT12_FILE_NAME_LEN]   = fname[++i];
    fmt_buf[MAX_FAT12_FILE_NAME_LEN+1] = fname[++i];
    fmt_buf[MAX_FAT12_FILE_NAME_LEN+2] = fname[++i];
}

fat_dentry_t * fat_find_entry_in_sector(uchar * entries, size_t re_size, const char * name){
    
    //check the valid file name 
    char * p = strchr(name, FILE_NAME_EXT_SEP_CHAR);
    size_t fname_len = p - name;
    if(p == NULL || fname_len> MAX_FAT12_FILE_NAME_LEN  || strlen(name)- fname_len != 1+MAX_FAT12_FILE_EXT_LEN){  
        FAT_ERROR(" file name:[%s] format error, must %d.%d style!\n", name, MAX_FAT12_FILE_NAME_LEN, MAX_FAT12_FILE_EXT_LEN);
        return NULL; 
    }

    //2. format the file name into storage format in dentry
    char buf[MAX_FAT12_FILE_NAME_LEN + MAX_FAT12_FILE_EXT_LEN+1];
    _format_file_name(buf, sizeof(buf), name, fname_len);
    FAT_DEBUG("after format the file name[%s] into [%s]\n", name, buf);
    
    //3. iterator all entries && cmp it
    size_t i = 0;
    for( i = 0; i < re_size; i+= 32)
    {
        fat_dentry_t * ptr =(fat_dentry_t*) (entries + i);
        print_n("dentry name & ext", ptr->name, MAX_FAT12_FILE_NAME_LEN + MAX_FAT12_FILE_EXT_LEN);
        if(strncmp(buf, ptr->name, sizeof(ptr->name)+sizeof(ptr->ext)) == 0)
            return  ptr;
    }
    return NULL;
}
