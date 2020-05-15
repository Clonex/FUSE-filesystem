#include <fuse.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_BLANK 69
#define DEFAULT_DIR_SIZE 10

#define ACCESS_READ 'r'
#define ACCESS_WRITE 'w'
#define ACCESS_READ_WRITE 'a'
#define ACCESS_EXEC 'e'


typedef struct entry {
	int type;
	void* data;

	int size;
	time_t time;
	char access; // read, write, read/write, execute permissions
	char name[];
} entry;