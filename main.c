#include "test_device.h"
#include "test_boot.h"


typedef union {
	int i;
	char buf[4];
} test_t;
static void test_fat12_cluster_val(){
	test_t t;
	t.buf[0] = 0x01;
	t.buf[1] = 0x02;
	t.buf[2] = 0x03;
	t.buf[3] = 0x04;
	printf("%x\n", t.i);



	char buf[6] = {0xff, 0x4f, 0x00, 0x05, 0xf0, 0xff};

	unsigned short int i;
	i = (*(unsigned short int*)buf);
	printf("0x%08x\n", i);
	printf("low  : %04x\n", i&0xfff);

	i = (*(unsigned short int*)(buf + 1));
	printf("0x%08x\n", i);
	printf("high : %04x\n", i>>4);


#define Clust_Val_First(addr)    ((*(unsigned short int*)(addr)) & 0xfff)
#define Clust_Val_Second(addr)    ((*(unsigned short int*)(addr+1)) >>4)
	printf("low  : %04x\n", Clust_Val_First(buf+3));
	printf("high : %04x\n", Clust_Val_Second(buf+3));
}


#include "buffer.h"
static void test_buffer(){
	buffer_t * ptr = buffer_create();
	buffer_init(ptr);
	buffer_show(ptr);
    buffer_free(ptr);
}

int main(int argc, char * argv[])
{
    test_device();
    test_boot();
    test_fat();
    test_dentry();

	test_buffer();
	return 0;
}
