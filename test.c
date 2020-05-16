#include "lfs.h" 
#include "implementation.c" 

void printArr(char **arr)
{
    printf("[");
    int length = sizeof(arr) / sizeof(arr[0]);
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
    char **test = splitString("/fil", '/');
    printf("Length = %d\n", count(test));
    printArr(test);
    //printf("%d\n", test);//a
}