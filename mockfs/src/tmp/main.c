#include "unit_test_wrapper.h"

#include "vfs.h"
#include "namespace.h"

TEST(MainTest, use){
    struct vfs * vfs = vfs_create();
    
    const char * dev_name = "./zero.img";
    const char * dir_name = "/mnt/";
    const char * type = "fat";
    ASSERT_EQ(0, ksys_mount(dev_name, dir_name, type, 0));
//
//
//    const char * fpath = "/hello.txt";
//    const int flags = R|W;
//    int fd = sys_open(fpath, flags);
//    ASSERT_TRUE(fd >= 0);
//
//    char buf[512] = {0};
//    int bytes = sys_read(fd, buf, 10);
//    ASSERT_TRUE(bytes > 0);
//
//    sys_close(fd);
//
    ASSERT_EQ(0, ksys_umount(dir_name, 0));

    vfs_destroy(vfs);
}


int main(int argc, char * argv[]){
    MainTest_use();
}
