#include "test_device.h"
#include "test_boot.h"

int main(int argc, char * argv[])
{
    test_device();
    
    test_boot();

    test_fat();

    test_dentry();
    return 0;
}
