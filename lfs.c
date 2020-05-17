#include <fuse.h>
#include "lfs.h" 
#include "implementation.c" 

int lfs_getattr( const char *, struct stat * );
int lfs_readdir( const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info * );
int lfs_open( const char *, struct fuse_file_info * );
int lfs_read( const char *, char *, size_t, off_t, struct fuse_file_info * );
int lfs_release(const char *path, struct fuse_file_info *fi);
int lfs_makefile(const char *path, mode_t mode, dev_t device);
int lfs_makedir(const char *path, mode_t mode);
void printArr(char **arr);

static inline int lsfs_utime_STUB(const char *path, struct utimbuf *buf) {
    (void)path;
    (void)buf;
    return 0;
}


static struct fuse_operations lfs_oper = {
	.getattr	= lfs_getattr,		// Get a attribute
	.readdir	= lfs_readdir,		//	
	.mknod 		= lfs_makefile,		// Make a file
	.mkdir 		= lfs_makedir,		// Make a directory
	.unlink 	= NULL,				// Remove file
	.rmdir 		= NULL,				// Rename folder
	.truncate 	= NULL,				// Empty a file
	.open		= lfs_open,			// Opens a 
	.read		= lfs_read,			// Reads a 
	.release 	= lfs_release,		// Closes a 
	.write 		= NULL,			
	.rename 	= NULL,				
	.utime 		= lsfs_utime_STUB
};

int lfs_makefile(const char *path, mode_t mode, dev_t device){
	return createEntry(path, TYPE_FILE);
}

int lfs_makedir(const char *path, mode_t mode){
	return createEntry(path, TYPE_DIR);
}

int lfs_getattr( const char *path, struct stat *stbuf ) {
	
	printf("getattr(): (path=%s)\n", path);

	memset(stbuf, 0, sizeof(struct stat));
	if( strcmp( path, "/" ) == 0 ) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else {
		char **tempPath = splitString(path, '/');
		if(tempPath == NULL)
		{
			return -1; // TODO
		}
		int length = getLength(tempPath);
		char *fileName = tempPath[length - 1];
		printf("getattr(): tempPath.length = %d\n", length);
		printArr(tempPath);
		
		printf("getattr(): Searching for file: %s\n", fileName);
		entry *file = findEntry(tempPath, root_fs);
		if(file != NULL)
		{
			printf("getattr(): Found attr-entry, name: %s\n", file->name);
			if(file->type == TYPE_DIR)
			{
				stbuf->st_mode = S_IFDIR | 0755;
			}else{
				stbuf->st_mode = S_IFREG | 0755;
			}
			stbuf->st_nlink = 1;
			stbuf->st_size = file->size;
			stbuf->st_mtime = file->time;
			return 0;
		}
		return -ENOENT;
		/*entry *files = (entry *) dir->data;
		for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
			entry file = files[fileI];
			if(file.type != TYPE_BLANK)
			{
				printf("getattr(): comparing name: %s\n", file.name);
			}
			if(file.type != TYPE_BLANK && strcmp(file.name, fileName) == 0){
				printf("Found attr-entry..\n");
				if(file.type == TYPE_DIR)
				{
					stbuf->st_mode = S_IFDIR | 0755;
				}else{
					stbuf->st_mode = S_IFREG | 0755;
				}
				stbuf->st_nlink = 1;
				stbuf->st_size = file.size;
				stbuf->st_mtime = file.time;
				return 0;
			}
		}*/
	}

	return 0;
}



int lfs_readdir( const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ) {
	(void) offset;
	(void) fi;
	printf("readdir(): (path=%s)\n", path);

	/*if(strcmp(path, "/") != 0)
		return -ENOENT;*/


	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	filler(buf, "hejsa", NULL, 0);

	char **tempPath = splitString(path, '/');
	printArr(tempPath);
	entry *dir = findEntry(tempPath, root_fs);
	printf("readdir(): name = %s\n\n", dir->name);
	entry *files = (entry *) dir->data;
	for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
		if(files[fileI].type != TYPE_BLANK){
			filler(buf, files[fileI].name, NULL, 0);
			printf("readdir(): file: %s\n", files[fileI].name);
		}
	}
	
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
	root_fs->name = "root";

    root_fs->size = sizeof(entry) * DEFAULT_DIR_SIZE;
    root_fs->data = calloc(DEFAULT_DIR_SIZE, sizeof(entry));
    if(root_fs->data == NULL)
    {
        return -1; // TODO: error
    }
    entry *data = (entry *) root_fs->data;
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        data[i].type = TYPE_BLANK;
		data[i].name = "";
    }

	fuse_main( argc, argv, &lfs_oper );
	return 0;
}


void printArr(char **arr)
{
    printf("[");
    int length = getLength(arr);
    for(int i = 0; i < length; i++)
    {
        if(i == 0)
        {
            printf("\"%s\"", arr[i]);
        }else{
            printf(" ,\"%s\"", arr[i]);
        }
    }
    printf("]\n");

}