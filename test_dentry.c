#include "test_dentry.h"

#include "dentry.h"

void test_dentry()
{
    fat_dentry_t dentry;
    memset(&dentry, 0, sizeof(fat_dentry_t));

    dentry.time.val = 0xb966;
    dentry.date.val = 0x42da;

    printf("time: %d:%d:%d \n", dentry.time.detail.hour, dentry.time.detail.minute, dentry.time.detail.second);
    printf("date: %d-%d-%d \n", dentry.date.detail.year+1980, dentry.date.detail.month, dentry.date.detail.day);
}
