
#ifndef  FAT_UTIL_H
#define  FAT_UTIL_H


#include <stddef.h>

char * _strsep(char **stringp, const char *delim);
char * trim(char *str);
char * ltrim(char *str);
char * rtrim(char *str);


#endif   /* FAT_UTIL_H */
