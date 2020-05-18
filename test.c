#include "lfs.h" 
#include "implementation.c" 
#include "disk.c" 

void printArr(char **arr)
{
    printf("[");
    int length = count(arr);
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


int count(char **arr)
{
    int count = -1;
    while(arr[++count]){}
    return count;
}

int main()
{
    root_fs = malloc(sizeof(struct entry));
	if(root_fs == NULL)
	{
		return -1; // TODO error
	}
	root_fs->name = "";
	root_fs->type = TYPE_DIR;
	root_fs->access = ACCESS_READ_WRITE;

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
        data[i].size = 0;
    }
    
    restoreFromDisk();

    //saveToDisk(root_fs);

    free(root_fs->data);
    free(root_fs);
    /*char *name;

    name = "";
    printf("length = %ld\n", sizeof(name));

    char test = "ksdfdlksfljklsdkjfjsalfæaslkjfhjklashdfælkjhdsafdlæhasdljkfhælasfhsæa";
    name = test;
    if(sizeof(name) > 32)
    {
        memcpy(name, name, 32);
    }
    
    printf("length = %ld\n", sizeof(name));*/


    //int test = createEntry("/fil", TYPE_FILE);
    /*
        /.
        /..
        /hej.txt
        /mappe
        /mappe/fil.txt
    */
    
    /*char **filTest = splitString("/hej.txt", '/');
    printf("fil1 Length = %d\n", count(filTest));
    printArr(filTest);

    char **filTest2 = splitString("/mappe", '/');
    printf("Length = %d\n", count(filTest2));
    printArr(filTest2);*/

    //createEntry("/test", TYPE_DIR);

    /*createEntry("/mappe", TYPE_DIR);
    createEntry("/mappe/hej.txt", TYPE_FILE);

    char *path = "/mappe";
    char **tempPath = splitString(path, '/');
	printArr(tempPath);
	entry *dir = findDir(tempPath, root_fs);
    printf("name = %s\n\n", dir->name);

    entry *files = (entry *) dir->data;
	for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
		if(files[fileI].type != TYPE_BLANK){
			printf("file: %s\n", files[fileI].name);
		}
	}*/
    //printf("%d\n", test);//a
}