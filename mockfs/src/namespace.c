#include "namespace.h"
#include "base.h"





struct path {
    //struct vfsmount * vfs;
    //struct dentry * dentry;
    void * vfs;
    void * dentry;
};

// sys_open:
//	path_openat(&nd, op_flags, flags);
//		struct file * file = alloc_empty_file(op->open_flags,current_cred());
//		else{
//			path_init();
//			while( link_path_walk(s, nd) && do_last(nd, file, op) ) {
//				s = trailing_symlink(nd);
//			}
//			terminate_walk(nd);
//		}
//		

//TODO: vfsmount root??? how to handle ????
int user_path_at(const char * dir_name, struct path * path){

    //user_path_at_empty(AT_FDCWD, name, flags, path, NULL);
    //filename_lookup(AT_FDCWD, getname_flags(name, flags, empty), flags, path, NULL);
	//	path_lookupat(nd, flags, path)
	//		path_init(nd, flags)
	//		while( link_path_walk(s, nd) &&  lookup_last(nd)) 
	//			s = trailing_symlink(nd)
	//
	//		terminate_walk(nd);
    //TODO:
    return 0;
}
void path_put(struct path * path){
    //TODO:
    // dput(path->dentry);
    // mntput(path->mnt);
}


int do_new_mount(struct path *path, const char * type, unsigned int sb_flags, unsigned int mnt_flags, const char *dev_name){
    //TODO:
    return 0;
}


int ksys_mount(const char * dev_name, const char * dir_name, const char * type, int flags){
	//super block init ;
	
    unsigned int mnt_flags = 0;
    unsigned int sb_flags = 0;

    struct path path;
    int retval = user_path_at(dir_name,&path);
    if(retval){
        pr_error("get path from dir_name(%s) string failed\n", dir_name);
        return retval;
    }

    retval = do_new_mount(&path, type, sb_flags, mnt_flags, dev_name);
    
dput_out:
    path_put(&path);
    return retval;
}
int ksys_umount(const char * dir_name, int flags){
    //TODO:
    return 0;
}
