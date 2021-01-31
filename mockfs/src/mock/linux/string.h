
#ifndef  LINUX_STRING_H
#define  LINUX_STRING_H

#include <string.h>

#undef strlcpy
#define strlcpy     strncpy

#endif   /* LINUX_STRING_H */
