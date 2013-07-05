#include "log.h"

#include <time.h>
#include <stdarg.h>

void PrintOut(const int moduleIndex, const int lvl, const char * fileName, const char * funcName, int line, const char * fmt, ...)
{
	static const char * moduleName[]={
		"FAT12",
	};
	static const char * levelName[] = {
		"FATAL",
		"ERROR",
		"WARN",
		"DEBUG",
		"INFO"
	};
	time_t now;                                                      
	char dbgtime[20] ;                                                  
	time(&now);                                                         
	struct tm cur_tm;
	localtime_r(&now, &cur_tm);
	strftime(dbgtime, sizeof(dbgtime),"%Y-%m-%d %H:%M:%S", &cur_tm);
	dbgtime[19] = '\0';       

	printf("%s %s %s:%d ", levelName[lvl], dbgtime, fileName, line);
	va_list pArg;
	va_start(pArg, fmt);
	vprintf(fmt, pArg);
	va_end(pArg);
}
