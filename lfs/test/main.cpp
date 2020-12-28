#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char * argv[]){
	const char * path = "/home/chenglun/ab.txt";

	if(argc > 1){
		printf("argv[1]:%s\n", argv[1]);
		path = argv[1];
	}

	//int flags = O_RDWR|O_CREAT;
	//int fd = open(path, flags);
	int fd = creat(path, 00644);
	if(fd < 0){
		printf("open failed, error_no:%d|%s\n", errno, strerror(errno));
		return -1;
	}
	printf("open ok\n");
	close(fd);

	return 0;
}
