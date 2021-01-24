#include "base.h"
#include "filesystem.h"
#include "super.h" //for mount_bdev
#include "inode.h"
#include "dcache.h"
#include "list.h"
#include "inode.h"
#include "buffer_head.h"

static int lfs_fill_super(struct super_block * sb, void * data, int silent)
{
    int err;
    struct inode * root_inode;

    struct buffer_head * mbr = sb_bread(sb, 0);
    if(!mbr){
        return -ENOMEM;
    }
    

    root_inode = new_inode(sb);
    if(!root_inode){
        err = -ENOMEM;
        pr_error("new inode for root failed, err:[%d]\n", err);
        goto out_fail;
    }

    //TODO:root_inode custom init; 
    
    sb->s_root = d_make_root(root_inode);
    if(!sb->s_root){
        err = -ENOMEM;
        pr_error("make root dentry failed, err:[%d]\n", err);
        goto out_fail; 
    }
    return 0;

out_fail:
	return err;
}

static struct dentry * lfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data){
	pr_debug("file system : lfs_mount .....\n");
	return mount_bdev(fs_type, flags, dev_name, data, &lfs_fill_super);
}
void lfs_kill_sb(struct super_block * sb){
	pr_debug("lfs_kill_sb\n");
	kill_block_super(sb);
}

static struct file_system_type lfs_fs_type = {
    .name       = "lfs",
    .mount      = lfs_mount,
    .kill_sb    = lfs_kill_sb,
    .fs_flags   = FS_REQUIRES_DEV,
};

static int lfs_init(){
	return register_filesystem(&lfs_fs_type);
}
static void lfs_exit(){
	unregister_filesystem(&lfs_fs_type);
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

	lfs_init();

	print_filesystem();

	root_dentry = lfs_fs_type.mount(&lfs_fs_type, 0, "/home/chenglun/zero.img", NULL);
    ASSERT_TRUE(root_dentry != NULL);
    
    sb = (root_dentry) ? root_dentry->d_sb : get_super_from_filesystem(&lfs_fs_type);

    if(sb != NULL){
        lfs_fs_type.kill_sb(sb);
    }
    else
        pr_error("sb is NULL, when kill sb\n");

	lfs_exit();
}

#include "list.h"

TEST(ListTest, use){
    LIST_HEAD(head);
}

int main(int argc, char * argv[]){
    FileSystemTypeTest_use();
    ListTest_use();
}
