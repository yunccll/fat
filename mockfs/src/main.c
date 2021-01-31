#include "mock/linux/list.h"
#include "mock/linux/errno.h"
#include "mock/linux/printk.h"
#include "mock/uapi/linux/fs.h"
#include "mock/uapi/linux/magic.h"
#include "mock/linux/byteorder/little_endian.h"
#include "mock/linux/byteorder/generic.h"
#include "mock/linux/fs.h"

#include "unit_test_wrapper.h"

#include "filesystem.h"
#include "super.h" //for mount_bdev
#include "inode.h"
#include "dcache.h"
#include "inode.h"
#include "block_dev.h"
#include "buffer_head.h"
#include "fat_super.h"
#include "fat_inode.h"
#include "fat_misc.h"


static struct super_operations fat_sb_ops= {
    .alloc_inode = fat_alloc_inode,
    //.destroy_inode = fat_destroy_inode;
    //.evict_inode = fat_evict_inode;
};


static void sb_init(struct super_block * sb)
{
	struct timespec64 ts;

	sb->s_flags |= SB_NODIRATIME|SB_NOATIME;
    sb->s_magic = MSDOS_SUPER_MAGIC;
    sb->s_op = &fat_sb_ops;
    //sb->s_export_op = &fat_export_ops;
	//TODO: fat_sb(sb)->dir_ops = &msdos_dir_inode_operations;
    //TODO: sb->s_d_op = &msdos_dentry_operations;

	sb->s_time_gran = 1; 
	sb_min_blocksize(sb, 512);
	sb->s_maxbytes = 0xffffffff;
	fat_time_fat2unix(fat_sb(sb), &ts, 0, cpu_to_le16(FAT_DATE_MIN), 0);
    sb->s_time_min = ts.tv_sec;

    fat_time_fat2unix(fat_sb(sb), &ts, cpu_to_le16(FAT_TIME_MAX), cpu_to_le16(FAT_DATE_MAX), 0);
    sb->s_time_max = ts.tv_sec;
}
static void fat_fs_info_free(struct super_block * sb)
{
    struct fat_sb * fsb = fat_sb(sb);
    if(fsb){
        fat_sb_free(fsb);
    }
}
static int fat_fs_info_load(struct super_block * sb)
{
    struct buffer_head * bh;
	struct fat_sb * fsb;
    assert(sb);
	bh = sb_bread(sb, 0);
    if(!bh){
        return -ENOMEM;
    }
	fsb = fat_sb_create((u8*)(bh->b_data), BLOCK_SIZE);
    fat_sb_save(sb, fsb);
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
	sb_init(sb);

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


TEST(ListTest, use){
    LIST_HEAD(head);
}

int main(int argc, char * argv[]){
    FileSystemTypeTest_use();
    ListTest_use();
}
