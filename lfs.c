#include "lfs.h" 
#include "implementation.c" 

int lfs_getattr( const char *, struct stat * );
int lfs_readdir( const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info * );
int lfs_open( const char *, struct fuse_file_info * );
int lfs_read( const char *, char *, size_t, off_t, struct fuse_file_info * );
int lfs_release(const char *path, struct fuse_file_info *fi);
int lfs_makefile(const char *path, mode_t mode, dev_t device);

static struct fuse_operations lfs_oper = {
	.getattr	= lfs_getattr,	// Get a attribute
	.readdir	= lfs_readdir,	//	
	.mknod = lfs_makefile,				// Make a file
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

int lfs_makefile(const char *path, mode_t mode, dev_t device){
	return createEntry(path, TYPE_FILE);
}

int lfs_getattr( const char *path, struct stat *stbuf ) {
	int res = 0;
	printf("getattr: (path=%s)\n", path);

	memset(stbuf, 0, sizeof(struct stat));
	if( strcmp( path, "/" ) == 0 ) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if( strcmp( path, "/folder" ) == 0 ) {
		stbuf->st_mode = S_IFREG | 0755;
		stbuf->st_nlink = 1;
		stbuf->st_size = 150;
	} else {
		res = -ENOENT;
	}

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
	filler(buf, "test", NULL, 0);
	filler(buf, "folder", NULL, 0);

	

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
	root_fs = malloc(sizeof(struct entry));
	if(root_fs == NULL)
	{
		return -1; // TODO error
	}
	root_fs->type = TYPE_DIR;
	root_fs->access = ACCESS_READ_WRITE;

	fuse_main( argc, argv, &lfs_oper );
	return 0;
}