#ifndef  NAMESPACE_H
#define  NAMESPACE_H

extern int ksys_mount(const char * dev_name, const char * dir_name, const char * type, int flags);
extern int ksys_umount(const char * dir_name, int flags);

#endif   /* NAMESPACE_H */
