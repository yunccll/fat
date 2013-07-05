
#ifndef  LOG_H
#define  LOG_H

#include <stdio.h>

#define FAT_FATAL(args...)       PrintOut(0, 0, __FILE__, __func__, __LINE__, ##args)
#define FAT_ERROR(args...)       PrintOut(0, 1, __FILE__, __func__, __LINE__, ##args)
#define FAT_WARNING(args...)	 PrintOut(0, 2, __FILE__, __func__, __LINE__, ##args)
#define FAT_DEBUG(args...)       PrintOut(0, 3, __FILE__, __func__, __LINE__, ##args)
#define FAT_PRINT(args...)       PrintOut(0, 4, __FILE__, __func__, __LINE__, ##args)

void PrintOut(const int moduleIndex, const int lvl, const char * fileName, const char * funcName, int line, const char * fmt, ...);

#endif   /* LOG_H */
