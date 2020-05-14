#include <fuse.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int lfs_getattr( const char *, struct stat * );
int lfs_readdir( const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info * );
int lfs_open( const char *, struct fuse_file_info * );
int lfs_read( const char *, char *, size_t, off_t, struct fuse_file_info * );
int lfs_release(const char *path, struct fuse_file_info *fi);

static struct node {
	char name[];
	bool isFolder;
	void* data;
	godNode attributes;
} node;

static struct godNode {
	int size;
	int time;
	char access; // read, write, execute permissions
} godNode;

static struct fuse_operations lfs_oper = {
	.getattr	= lfs_getattr,	// Get a attribute
	.readdir	= lfs_readdir,	//	
	.mknod = NULL,				// Make a file
	.mkdir = NULL,				// Make a directory
	.unlink = NULL,				// Remove file
	.rmdir = NULL,				// Rename folder
	.truncate = NULL,			// Empty a file
	.open	= lfs_open,			// Opens a 
	.read	= lfs_read,			// Reads a 
	.release = lfs_release,		// Closes a 
	.write = NULL,			
	.rename = NULL,				
	.utime = NULL
};

node root_fs;

int lfs_getattr( const char *path, struct stat *stbuf ) {
	int res = 0;
	printf("getattr: (path=%s)\n", path);

	memset(stbuf, 0, sizeof(struct stat));
	if( strcmp( path, "/" ) == 0 ) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if( strcmp( path, "/hello" ) == 0 ) {
		stbuf->st_mode = S_IFREG | 0777;
		stbuf->st_nlink = 1;
		stbuf->st_size = 12;
	} else
		res = -ENOENT;

	return res;
}

int lfs_readdir( const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ) {
	(void) offset;
	(void) fi;
	printf("readdir: (path=%s)\n", path);

	if(strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, "hello", NULL, 0);

	return 0;
}

//Permission
int lfs_open( const char *path, struct fuse_file_info *fi ) {
    printf("open: (path=%s)\n", path);
	return 0;
}

int lfs_read( const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi ) {
    printf("read: (path=%s)\n", path);
	memcpy( buf, "Hello\n", 6 );
	return 6;
}

int lfs_release(const char *path, struct fuse_file_info *fi) {
	printf("release: (path=%s)\n", path);
	return 0;
}

int main( int argc, char *argv[] ) {
	root_fs = malloc(sizeof(node));
	printf("fuck um");
	fuse_main( argc, argv, &lfs_oper );
	return 0;
}