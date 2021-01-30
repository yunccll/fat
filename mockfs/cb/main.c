#include "base.h"
#include "filesystem.h"
#include "super.h" //for mount_bdev
#include "inode.h"
#include "dcache.h"
#include "list.h"
#include "inode.h"
#include "buffer_head.h"
#include "fat_super.h"
#include "fat_inode.h"

static struct super_operations fat_sb_ops= {
    .alloc_inode = fat_alloc_inode,
    //.destroy_inode = fat_destroy_inode;
    //.evict_inode = fat_evict_inode;
};


static void fat_fs_info_free(struct super_block * sb){
    struct fat_sb * sbf = fat_sb(sb);
    if(sbf){
        fat_sb_free(sbf);
    }
}
static int fat_fs_info_load(struct super_block * sb)
{
    assert(sb);
    struct buffer_head * bh = sb_bread(sb, 0);
    if(!bh){
        return -ENOMEM;
    }
    struct fat_sb * sbf = fat_sb_create((u8*)(bh->b_data), BLOCK_SIZE);
    fat_sb_save(sb, sbf);
    brelse(bh);
    return 0;
}


static void fat_inode_free(struct super_block * sb)
{
}
static int fat_inode_load(struct super_block * sb)
{
    //1<->9
    //struct buffer_head *fat1 = sb_bread(sb, 1);
    //TODO: test the code for fat1
    //10->18
    //struct buffer_head *fat2 = sb_bread(sb, 10);
    //TODO: test the code for fat2
    return 0;
}

static int root_dir_load(struct super_block * sb)
{
    int err = -ENOMEM ;
    struct inode * root_inode = new_inode(sb);
    if(!root_inode){
        pr_error("new inode for root failed, err:[%d]\n", err);
        return -ENOMEM;
    }

    //TODO:root_inode custom init; 
    
    sb->s_root = d_make_root(root_inode);
    if(!sb->s_root){
        err = -ENOMEM;
        pr_error("make root dentry failed, err:[%d]\n", err);
        goto root_inode_free;
    }
    return 0;

root_inode_free:
    iput(root_inode);
    return err; 
}

static int fat_fill_super(struct super_block * sb, void * data, int silent)
{
    int err;
    //0. sb init
    sb->s_op = &fat_sb_ops;

    //1. fs_info_inode
    err = fat_fs_info_load(sb);
    if(err){
        pr_error("fs_info load failed, err:[%d]\n", err);
        return err;
    }
    //2. fat  inode
    err = fat_inode_load(sb);
    if(err){
        pr_error("fat_inode load failed, err:[%d]\n",err);
        goto out_fail;
    }
    //3. root_directory inode
    err = root_dir_load(sb);
    if(err){
        pr_error("root dir load failed, err:[%d]\n", err);
        goto  out_fat_inode_fail;
    }
    return 0;

out_fat_inode_fail:
    fat_inode_free(sb);
out_fail:
    fat_fs_info_free(sb);
    return err;
}

static struct dentry * fat_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data){
	pr_debug("file system : fat_mount .....\n");
	return mount_bdev(fs_type, flags, dev_name, data, &fat_fill_super);
}
void fat_kill_sb(struct super_block * sb){
	pr_debug("fat_kill_sb\n");
    fat_sb_free(fat_sb(sb));
    sb->s_fs_info = NULL;
	kill_block_super(sb);
}

static struct file_system_type fat_fs_type = {
    .name       = "fat",
    .mount      = fat_mount,
    .kill_sb    = fat_kill_sb,
    .fs_flags   = FS_REQUIRES_DEV,
};

static int fat_init(){
	return register_filesystem(&fat_fs_type);
}
static void fat_exit(){
	unregister_filesystem(&fat_fs_type);
}

struct super_block * get_super_from_filesystem(struct file_system_type * fs_type){
	if(fs_type && fs_type->fs_supers.first){
		return (struct super_block*)hlist_entry(fs_type->fs_supers.first, struct super_block, s_instances);
	}
	return 0;
}

TEST(FileSystemTypeTest, use){
    struct dentry * root_dentry;
    struct super_block * sb;

	fat_init();

	print_filesystem();

	root_dentry = fat_fs_type.mount(&fat_fs_type, 0, "zero.img", NULL);
    ASSERT_TRUE(root_dentry != NULL);
    
    sb = (root_dentry) ? root_dentry->d_sb : get_super_from_filesystem(&fat_fs_type);

    if(sb != NULL){
        fat_fs_type.kill_sb(sb);
    }
    else
        pr_error("sb is NULL, when kill sb\n");

	fat_exit();
}

#include "list.h"

TEST(ListTest, use){
    LIST_HEAD(head);
}

int main(int argc, char * argv[]){
    FileSystemTypeTest_use();
    ListTest_use();
}
