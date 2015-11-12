
#ifndef  LOG_H
#define  LOG_H

#include <stdio.h>

#define PRINT_OUT      printf

#define FAT_FATAL(args...)       print_out(0, 0, __FILE__, __func__, __LINE__, ##args)
#define FAT_ERROR(args...)       print_out(0, 1, __FILE__, __func__, __LINE__, ##args)
#define FAT_WARNING(args...)	 print_out(0, 2, __FILE__, __func__, __LINE__, ##args)
#define FAT_DEBUG(args...)       print_out(0, 3, __FILE__, __func__, __LINE__, ##args)
#define FAT_PRINT(args...)       print_out(0, 4, __FILE__, __func__, __LINE__, ##args)

void print_out(const int moduleIndex, const int lvl, const char * fileName, const char * funcName, int line, const char * fmt, ...);
void print_n(const char * msg, const char * str, size_t n);

#endif   /* LOG_H */
