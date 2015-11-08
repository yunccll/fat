#include "test_dentry.h"

#include "dentry.h"
#include "device.h"


static void test_root_entries_print()
{
    FAT_PRINT("Root Entries info :\n");
	int ret = 0;
    const char * file_name = "a.img.flp";

    //1. alloc mem
	size_t re_size = 224 * 32;
	uchar * pre = (uchar *) malloc(sizeof(uchar) * re_size);
	assert(pre);

    //2. read dev with root directory sectors 
    DECLARE_DEVICE_R(fdev, file_name);
    ret = fat_dev_read(&fdev, 19*512, (uchar*)pre, re_size);
	assert(ret == re_size);
    fat_dev_destroy(&fdev);

    //3. print the entries
	fat_root_entries_print(pre, re_size);

    const char * fn = "A.TXT";
    FAT_DEBUG("find the file name is [%s]\n", fn);
    fat_dentry_t  * ptr =fat_find_entry_in_sector(pre, re_size, fn);
    if(NULL != ptr){
        FAT_DEBUG("find the dentry with file_name 0x%X\n", ptr);
    }
    else{
        FAT_DEBUG("can not find the dentry with file_name [%s]\nn",fn);
    }

    //4. free the mem 
	free(pre); pre = NULL;
    FAT_PRINT("Finished......\n\n");
}

void test_dentry()
{
    test_root_entries_print();
}
