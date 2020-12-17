#include <linux/module.h>
static int __init hello_init(void){
    printk(KERN_INFO "Hello,%s\n", KBUILD_MODNAME);
    return 0;
}
static void __exit hello_exit(void){
    printk(KERN_INFO "Bye,%s\n", KBUILD_MODNAME);
}


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello template");
MODULE_AUTHOR("Cheng Lun");

module_init(hello_init);
module_exit(hello_exit);
