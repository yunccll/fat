#include "filesystem.h"
#include "base.h"


static struct file_system_type * file_systems = 0;

static struct file_system_type ** find_filesystem(const char * name, unsigned int len)
{
	struct file_system_type **p; 
    for (p = &file_systems; *p; p = &(*p)->next) 
        if (strncmp((*p)->name, name, len) == 0 && !(*p)->name[len]) 
            break; 
    return p; 
}

int register_filesystem(struct file_system_type * fs)
{
	int res = 0;
	struct file_system_type ** p;

	if(fs->next){
		return -EBUSY;
	}
	p = find_filesystem(fs->name, strlen(fs->name));
    if(*p){
        res = -EBUSY;
	}
	else{
		*p =fs;
	}
	return res;
}
void print_filesystem(void)
{
	struct file_system_type * p;
	for(p = file_systems; p; p = p->next){
		pr_info("file system list:%s\n", p->name);
	}
}
int unregister_filesystem(struct file_system_type * fs)
{
	struct file_system_type ** tmp = &file_systems;
    while (*tmp) {
        if (fs == *tmp) {
            *tmp = fs->next;
            fs->next = NULL;
            //TODO: synchronize_rcu();
            return 0;
        }
        tmp = &(*tmp)->next;
    }

	return -EINVAL;
}
struct file_system_type * get_filesystem(struct file_system_type * fs_type){
    //__module_get(fs_type->owner); 
    return fs_type;
}
void put_filesystem(struct file_system_type * fs_type){
    //module_put(fs_type->owner);
}
