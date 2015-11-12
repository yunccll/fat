#include "fat_util.h"
#include <stddef.h>
#include <string.h>

char *_strsep(char **stringp, const char *delim)
{
    char *s; 
    const char *spanp;
    int c, sc; 
    char *tok;
    if ((s = *stringp)== NULL)
        return (NULL);
    for (tok = s;;) {
        c = *s++;
        spanp = delim;
        do {
            if ((sc =*spanp++) == c) {
                if (c == 0)
                    s = NULL;
                else
                    s[-1] = 0;
                *stringp = s;
                return (tok);
            }   
        } while (sc != 0); 
    }   
    /*   NOTREACHED */
}
char * trim(char *str) {
    return ltrim(rtrim(str));
}

char * ltrim(char *str) {
    char *copied = str, *tail = str;
    while (' ' == *tail || '\t' == *tail || '\n' == *tail || '\r' == *tail) tail++;
    if (tail == copied) return str;
    while (*tail) *copied++ = *tail++;
    *copied = '\0';
    return str;
}

char * rtrim(char *str) {
    size_t j = strlen(str);
    while (j && ((' ' == str[j - 1]) || ('\t' == str[j - 1]) || ('\n' == str[j - 1]) || ('\r' == str[j - 1]))) j--;
    str[j]='\0';
    return str;
}
