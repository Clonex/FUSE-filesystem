entry* findDir(char **pathArr, entry* folder);
char **splitString(const char *input, char delimiter);
int getLength(char **arr);

/*
 * 
 */

/*
 * Adds a entry to the correct folder.
 */
int createEntry(const char *path, int type)
{
    printf("Creating file..\n");
    entry *file = calloc(1, sizeof(entry));
    if(file == NULL){
        return -1; // TODO error
    }
    if(type == TYPE_DIR)
    {
        file->size = sizeof(entry) * DEFAULT_DIR_SIZE;
        file->data = calloc(DEFAULT_DIR_SIZE, sizeof(entry));
        if(file->data == NULL)
        {
            return -1; // TODO: error
        }
        entry *data = (entry *) file->data;
        for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
        {
            data[i].type = TYPE_BLANK;
            data[i].name = "";
        }
    }else{
        file->size = 0;
    }
    time_t stamp;
    time(&stamp);
    file->time = stamp;
    file->type = type;
  
    char **pathArr = splitString(path, '/');
    if(pathArr == NULL)
    {
        return -1; // TODO: error
    }
    file->name = pathArr[getLength(pathArr) - 1]; 
    file->access = ACCESS_READ_WRITE;

    entry *targetDir = findDir(pathArr, root_fs);
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        entry *data = (entry *) targetDir->data;
        if(data[i].type == TYPE_BLANK)
        {
            data[i] = *file;
            break;
        }
    }
    printf("Entry added..\n");
    return 0;
}

/*
 * Finds the correct entry for a given path.
 */
entry* findDir(char **pathArr, entry* folder){
    size_t length = getLength(pathArr);

    printf("Checking dir..\n");
    entry *currentEntry = folder;
    for(int pathI = 0; pathI < length - 1; pathI++){
        printf("Current dir: %s\n", currentEntry->name);
        printf("Current path: %s\n", pathArr[pathI]);

        for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
            entry *file = (entry *) currentEntry->data;
            if(file[fileI].type == TYPE_DIR && strcmp(file[fileI].name, pathArr[pathI]) == 0){
                currentEntry = &file[fileI];
                printf("New folder: %s\n", currentEntry->name);
                break; // Next path
            }
        }
        printf("\n");
    }
    return currentEntry;
}

/*
 * @returns a array of char array
 */
char **splitString(const char *input, char delimiter){
    int count = 0;
    int j = 0;
    char **out = calloc(strlen(input), sizeof(char *));
    if(out == NULL)
    {
        return NULL;
    }
    for(int i = 0; i < strlen(input); i++)
    {
        if(input[i] == delimiter)
        {
            out[count] = calloc((i - j), sizeof(char));
            if(out[count] == NULL)
            {
                return NULL;
            }
            memcpy(out[count], &input[j], (i-j));
            count++;
            j = i+1;
        }
    }
    out[count] = calloc(((strlen(input) - 1) - j), sizeof(char));
    memcpy(out[count], &input[j], (strlen(input) - j));

    return out;
}

/*
 * 
 */
int getLength(char **arr)
{
    int count = -1;
    while(arr[++count]){}
    return count;
}