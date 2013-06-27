#include "boot.h"


static fat_boot_ops_t boot_ops = {
    .print = fat_boot_print,
};

static fat_boot_t boot = {
    .i = 12,
    .ops = &boot_ops,
};


int main(int argc, char * argv[])
{
    boot.ops->print(&boot); 
    return 0;
}
