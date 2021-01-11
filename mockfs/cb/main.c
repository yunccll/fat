#include "base.h"
#include "mock.h"
#include "filesystem.h"
#include "super.h" //for mount_bdev

static int lfs_fill_super(struct super_block * sb, void * data, int silent)
{
	//TODO:
	return 0;
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


TEST(FileSystemTypeTest, use){
	lfs_init();

	print_filesystem();

	lfs_fs_type.mount(&lfs_fs_type, 0, "~/zero.img", NULL);

	struct super_block sb = {
	};
	lfs_fs_type.kill_sb(&sb);

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
