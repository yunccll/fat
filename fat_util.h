
#ifndef  FAT_UTIL_H
#define  FAT_UTIL_H


#include <stddef.h>

char * strsep(char **stringp, const char *delim);
char * trim(char *str);
char * ltrim(char *str);
char * rtrim(char *str);
size_t index_of(const char * str, const char ch);


#endif   /* FAT_UTIL_H */
