#ifndef FILE_H
#define FILE_H

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>

// if NULL, check errno!
// else don't forget to free!
char* read_file(std::string filename){
	int fd = open(filename.c_str(),O_RDONLY);
	if(fd==-1) return NULL;
	static struct stat filestat;
	if(fstat(fd,&filestat)==-1) return NULL;
	size_t const size = (size_t)filestat.st_size;
	char* str = (char*)malloc(size);
	if(read(fd,str,size)!=(ssize_t)size){free(str); return NULL;}
	if(close(fd)==-1){free(str); return NULL;}
	return str;
}

#endif
