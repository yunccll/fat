#include <linux/module.h>
#include "base.h"


#define MAX_FILE_NAME_SIZE  8
#define MAX_DIR_ITEM_SIZE       (512/(MAX_FILE_NAME_SIZE))


struct lfs_dir_item_node {
    struct hlist_node list;
    char name[MAX_FILE_NAME_SIZE];
};

struct lfs_dir_item_cache {
    struct hlist_head hash[MAX_DIR_ITEM_SIZE];
    uint32_t    (*fn_hash)(const char * name);
    uint32_t used;
    //struct kmem_cache       *dir_item_cachep;
    //const char * dir_item_cache_name;
};


static inline u64 hash_64(u64 val, unsigned int bits)
{
    u64 hash = val;

    /*   Sigh, gcc can't optimise this alone like it does for 32 bits. */
    u64 n = hash;
    n <<= 18;
    hash -= n;
    n <<= 33;
    hash -= n;
    n <<= 3;
    hash += n;
    n <<= 3;
    hash -= n;
    n <<= 4;
    hash += n;
    n <<= 2;
    hash += n;

    /*  High bits are more random, so use them. */
    return hash >> (64 - bits);
}
static uint32_t hash(const char * name){
    char buf[MAX_FILE_NAME_SIZE] = {0};
    strncpy(buf, name, sizeof(buf));
    return (uint32_t)(hash_64(*(uint64_t*)(buf), 6) & 0x3F);
}





struct lfs_dir_item_cache * root;

// 2. free
static void lfs_dir_item_cache_free(struct lfs_dir_item_cache * c){
    //free all hash nodes
    struct lfs_dir_item_node * p;
    struct hlist_node * n;
    int i = 0;
    for(; i < MAX_DIR_ITEM_SIZE; ++i){
        if(!hlist_empty(c->hash+i)){ //not null => foreach & print it
            hlist_for_each_entry_safe(p, n, (c->hash+i), list){
                pr_info("free name is :%s", p->name);
                hlist_del(&(p->list));
                kfree(p);
            }
        }
    }
    //free myself
    if(c != NULL){
        kfree(c);
    }
}

// 1. create
static struct lfs_dir_item_cache * lfs_dir_item_cache_create(void){
    int i = 0;
    struct lfs_dir_item_cache * c = kzalloc(sizeof(struct lfs_dir_item_cache), GFP_KERNEL);
    if(c != NULL){
        for(; i < MAX_DIR_ITEM_SIZE; ++i){
            INIT_HLIST_HEAD(c->hash + i);
        }
        c->fn_hash = &hash;
        return c;
    }
    return NULL;
}
// 3. insert 
static int lfs_dir_item_cache_insert_node(struct lfs_dir_item_cache * c, struct lfs_dir_item_node * node){
    if(node != NULL && hlist_unhashed(&(node->list))){
        uint32_t hv = c->fn_hash(node->name);
        hlist_add_head(&(node->list), c->hash + hv);
        c->used++;
        return 0;
    }
    return -1;
}
static int lfs_dir_item_cache_insert(struct lfs_dir_item_cache * c, const char * name){
    if(name != NULL){
        struct lfs_dir_item_node * node = kzalloc(sizeof(struct lfs_dir_item_node), GFP_KERNEL);
        if(node != NULL){
            INIT_HLIST_NODE(&(node->list));
            strncpy(node->name, name, MAX_FILE_NAME_SIZE);
            return lfs_dir_item_cache_insert_node(c, node);
        }
        return -ENOMEM;
    }
    return -1;
}

// 0. print
static void lfs_dir_item_cache_print(struct lfs_dir_item_cache * c){
    if(c != NULL){
        struct lfs_dir_item_node * p;
        int i = 0;
        for(; i < MAX_DIR_ITEM_SIZE; ++i){
            if(!hlist_empty(c->hash+i)){ //not null => foreach & print it
                hlist_for_each_entry(p, (c->hash+i), list){
                    pr_info("name is :%s", p->name);
                }
            }
        }
    }
    else{
        pr_info("cache is NULL");
    }
}
// 7. find
static struct lfs_dir_item_node * lfs_dir_item_cache_find(struct lfs_dir_item_cache *c , const char * name){
    if(c != NULL && name != NULL){
        uint32_t hv = c->fn_hash(name); 
        pr_info("hlist_empty:%d, hv:%d", hlist_empty(c->hash+hv), hv);

        if(!hlist_empty(c->hash + hv)){
            struct lfs_dir_item_node * p;
            hlist_for_each_entry(p, c->hash + hv, list){
                pr_info("p->name:%s", p->name);
                if(0 == strncmp(p->name, name, MAX_FILE_NAME_SIZE)){
                    return p; 
                }
            }
        }
    }
    return NULL;
}

// 4. contains  1-Yes 0-No
static int lfs_dir_item_cache_contains(struct lfs_dir_item_cache * c, const char * name){
    return lfs_dir_item_cache_find(c, name) != NULL;
}

// 5. erase 
static int lfs_dir_item_cache_erase(struct lfs_dir_item_cache * c, const char * name){
    if(c != NULL && name != NULL){
        struct lfs_dir_item_node * p = lfs_dir_item_cache_find(c, name);
        if(p != NULL){
            hlist_del(&(p->list));
            --(c->used);
            kfree(p);
            return 0;
        }
        else{
            pr_info("can not find ...");
        }
    }
    return -1;
}
// 6. iterate see the lfs_dir_item_cache_print function 



static int __init hash_list_init(void){
    const char buf[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

    pr_info("hello,%s\n", KBUILD_MODNAME);
    root = lfs_dir_item_cache_create();
    if(root == NULL){
        pr_err("root is null for dir item cache");
        return -1;
    }
    pr_info("hash:%d", hash(buf));
    pr_info("h11111 not in hashlist, ret:%d", lfs_dir_item_cache_contains(root, "hllll"));

    lfs_dir_item_cache_insert(root, "hello");
    pr_info("hello in hashlist, ret:%d", lfs_dir_item_cache_contains(root, "hello"));

    pr_info("hash node count:%d",  root->used);

    lfs_dir_item_cache_print(root);

    lfs_dir_item_cache_erase(root, "hello");
    pr_info("----after erase node[hello], hash node count:%d",  root->used);
    lfs_dir_item_cache_print(root);

    return 0;
}
static void __exit hash_list_exit(void){
    lfs_dir_item_cache_free(root);
    pr_info("Bye,%s\n", KBUILD_MODNAME);
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HashList");
MODULE_AUTHOR("Cheng Lun");

module_init(hash_list_init);
module_exit(hash_list_exit);
