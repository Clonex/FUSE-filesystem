#include "lfs.h" 
#include "implementation.c" 

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
    }

    //int test = createEntry("/fil", TYPE_FILE);
    /*
        /.
        /..
        /hej.txt
        /mappe
        /mappe/fil.txt
    */
    char **filTest = splitString("/hej.txt", '/');
    printf("fil1 Length = %d\n", count(filTest));
    printArr(filTest);

    char **filTest2 = splitString("/mappe", '/');
    printf("Length = %d\n", count(filTest2));
    printArr(filTest2);
    //printf("%d\n", test);//a
}