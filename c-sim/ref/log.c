#include "log.h"
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <stdarg.h>

static char log_buffer[512];

void print_out(const int moduleIndex, const int lvl, const char * fileName, const char * funcName, int line, const char * fmt, ...)
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
	// get current time
	time_t now;                                                      
	char dbgtime[20] ;                                                  
	time(&now);                                                         
	struct tm cur_tm;
	localtime_r(&now, &cur_tm);
	strftime(dbgtime, sizeof(dbgtime),"%Y-%m-%d %H:%M:%S", &cur_tm);
	dbgtime[19] = '\0';       

	// print the prefix
	int pos = snprintf(log_buffer, sizeof(log_buffer), "%s %s %s:%d ", levelName[lvl], dbgtime, fileName, line);
	// print the custome format string
	va_list pArg;
	va_start(pArg, fmt);
	vsnprintf(log_buffer + pos, sizeof(log_buffer) - pos, fmt, pArg);
	va_end(pArg);
	// print all out
	if(lvl <= 1)
		//fprintf(stderr, "%s", log_buffer, pos );
		fprintf(stderr, "%s", log_buffer);
	else
		//fprintf(stdout, "%s", log_buffer, pos );
		fprintf(stdout, "%s", log_buffer);
}



void print_n(const char * msg, const char * str, size_t n){
    char * buf = (char *)calloc(n+1, sizeof(char));
    memcpy(buf, str, n);
    FAT_DEBUG("%s:[%s]\n", msg, buf);
    free(buf);
}
