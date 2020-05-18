#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>

#define TYPE_FILE 1
#define TYPE_DIR 2
#define TYPE_BLANK 69

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

int lfs_getattr( const char *, struct stat * );
int lfs_readdir( const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info * );
int lfs_open( const char *, struct fuse_file_info * );
int lfs_read( const char *, char *, size_t, off_t, struct fuse_file_info * );
int lfs_release(const char *path, struct fuse_file_info *fi);
int lfs_makefile(const char *path, mode_t mode, dev_t device);
int lfs_makedir(const char *path, mode_t mode);
int lfs_write( const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi );
int lsfs_truncate(const char *path, off_t offset);
int lsfs_unlink(const char *path);
int lsfs_rmdir(const char *path);

void printArr(char **arr);