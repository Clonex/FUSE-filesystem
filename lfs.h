#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_BLANK 3

#define DEFAULT_DIR_SIZE 10			// amount
#define DEFAULT_NAME_SIZE 32 		// length

#define ACCESS_READ 'r'
#define ACCESS_WRITE 'w'
#define ACCESS_READ_WRITE 'a'
#define ACCESS_EXEC 'e'

typedef struct entry {
	int type;
	void* data;

	int size;
	time_t modTime;
	time_t accessTime;
	char access; // read, write, read/write, execute permissions
	char *name;
} entry;

entry* root_fs;


int countFolders(entry *file);
char *createBlock(entry file);
char *pad(char *value, int length, char padding, bool leftPad);

entry* findEntry(char **pathArr);
char **splitString(const char *input, char delimiter, bool includeLast);
int getLength(char **arr);
void cutName(char* source, char *target);

void printArr(char **arr);

void saveToDisk(entry *file);
void restoreFromDisk();
char *removePadding(char *value, char padding, bool leftPad);