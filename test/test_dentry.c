#include "test_dentry.h"

#include "dentry.h"
#include "device.h"


static void test_root_entries_print()
{
    FAT_PRINT("Root Entries info :\n");
	int ret = 0;
    const char * file_name = "a.img.flp";

	size_t re_size = 224 * 32;

	uchar * pre = (uchar *) malloc(sizeof(uchar) * re_size);
	assert(pre);

    DECLARE_DEVICE_R(fdev, file_name);
    ret = fat_dev_read(&fdev, 19*512, (uchar*)pre, re_size);
	assert(ret == re_size);

	fat_root_entries_print(pre, re_size);
	free(pre); pre = NULL;
    FAT_PRINT("Finished......\n\n");
}

void test_dentry()
{
    test_root_entries_print();
}
