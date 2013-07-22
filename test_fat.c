#include "test_fat.h"

#include "fat.h"
#include "device.h"

void test_offset_to_clusno()
{
    // byte offset => clusno
    int ret = offset_to_clusno(0);
    assert(ret == 0);

    ret = offset_to_clusno(1);
    assert(ret == 1);
    ret = offset_to_clusno(2);
    assert(ret == 1);

    ret = offset_to_clusno(3);
    assert(ret == 2);
    
    ret = offset_to_clusno(4);
    assert(ret == 3);
    ret = offset_to_clusno(5);
    assert(ret == 3);
    
    ret = offset_to_clusno(6);
    assert(ret == 4);
    
    ret = offset_to_clusno(7);
    assert(ret == 5);
    ret = offset_to_clusno(8);
    assert(ret == 5);
}


void test_clusno_to_offset()
{
    int ret = clusno_to_offset(0);
    assert(ret == 0);

    ret = clusno_to_offset(1);
    assert(ret == 1);

    ret = clusno_to_offset(2);
    assert(ret == 3);

    ret = clusno_to_offset(3);
    assert(ret == 4);

    ret = clusno_to_offset(4);
    assert(ret == 6);

    ret = clusno_to_offset(5);
    assert(ret == 7);
}

void test_clusno_to_sectno()
{
    int ret = clusno_to_sectno(0);
    assert(31 == ret);

    ret = clusno_to_sectno(1);
    assert(32 == ret);

    ret = clusno_to_sectno(2);
    assert(33 == ret);
}

void test_sectno_to_clusno()
{
    int ret = sectno_to_clusno(33);
    assert(ret == 2);

    ret = sectno_to_clusno(32);
    assert(ret == 1);

    ret = sectno_to_clusno(31);
    assert(ret == 0);
}

void test_fat_stat_fat_info()
{
	int ret = 0;

    const char * file_name = "a.img.flp";
	size_t fat_size = 9 * 512 ;

	uchar * pb = (uchar *) malloc(sizeof(uchar)* fat_size);
	assert(pb);

    DECLARE_DEVICE_R(fdev, file_name);
    ret = fat_dev_read(&fdev, 512, (uchar*)pb, fat_size);
	assert(ret == fat_size);

	ret = fat_stat_fat_info(pb, fat_size);
	assert(ret == 0);
	free(pb); pb = NULL;
}

void test_fat()
{
    test_offset_to_clusno();
    test_clusno_to_offset();

    test_clusno_to_sectno();
    test_sectno_to_clusno();

	test_fat_stat_fat_info();
}
