#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s:%s:%d: " fmt, KBUILD_MODNAME, __func__, __LINE__


struct my_user {
    struct list_head list;
    int id;
    char name[8];
};

static struct list_head root;


//destroy
//static void destroyOne(struct list_head * node){
//    if(node != NULL){
//        struct my_user * user = list_entry(node, struct my_user, list);
//        list_del(node);
//        kfree(user);
//    }
//}
static void destroy(struct list_head * list){
    if(list != NULL){
        struct my_user * user;
        struct my_user * next;
        list_for_each_entry_safe(user, next, list, list){
            list_del(&(user->list));
            kfree(user);

        }

    }
}

static struct list_head * createOne(int id, const char * name){
    struct my_user * mu = kzalloc(sizeof(struct my_user), GFP_KERNEL);
    if(NULL == mu){
        pr_err("alloc memory failed");
        return NULL;
    }
    mu->id = id;
    strlcpy(mu->name, name, sizeof(mu->name));
    INIT_LIST_HEAD(&(mu->list));
    return &mu->list;
}

// visit one
static void printOne(struct list_head* node){
    if(node != NULL){
        struct my_user * user = list_entry(node, struct my_user, list);
        pr_info("id:%d, name:%s", user->id, user->name);
    }
    else{
        pr_info("user is null");
    }
}

//foreach
static void print(struct list_head * list){
    if(list != NULL){
        struct list_head * p = NULL;
        struct my_user * u = NULL;
        list_for_each(p, list){
            u = list_entry(p, struct my_user, list);
            printOne(p);
        }
    }
}


static int __init double_list_init(void){
    printk(KERN_INFO "Hello,%s\n", KBUILD_MODNAME);

    INIT_LIST_HEAD(&root);

    pr_info("list is empty:%d, ", list_empty(&root)); //head only -> :1
    pr_info("list is single:%d, ", list_is_singular(&root)); //head only :0

    {
        struct list_head * one = createOne(1, "haha");
        struct list_head * two = createOne(2, "hhhhh");
        struct list_head * three = createOne(3, "chenglun");

        if(one != NULL && two != NULL && three != NULL){
            printOne(one);
            //list_add(one, &root); //first node
            list_add_tail(one, &root); //first node
            pr_info("list is single:%d", list_is_singular(&root));

            printOne(two);
            //list_add(two, &root); //second node
            list_add_tail(two, &root); //second node
            pr_info("list is single:%d", list_is_singular(&root));

            printOne(three);
            //list_add(three, &root); //third node
            list_add_tail(three, &root); //third node
            pr_info("list is single:%d", list_is_singular(&root));

            print(&root);

        }
        else{
            pr_err("create users failed");
        }
    }

    return 0;
}
static void __exit double_list_exit(void){
    pr_info("before destroy, list is empty:%d, ", list_empty(&root)); //head only -> :1
    destroy(&root);
    pr_info("after destroy, list is empty:%d, ", list_empty(&root)); //head only -> :1
    printk(KERN_INFO "Bye,%s\n", KBUILD_MODNAME);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello template");
MODULE_AUTHOR("Cheng Lun");

module_init(double_list_init);
module_exit(double_list_exit);
