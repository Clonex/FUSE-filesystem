entry* findDir(char **pathArr, entry* folder);
char **splitString(const char *input, char delimiter);

/*
 * 
 */

/*
 * Adds a entry to the correct folder.
 */
int createEntry(const char *path, int type)
{
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
    size_t length = sizeof(pathArr) / sizeof(pathArr[0]);
    file->name = pathArr[length - 1]; 
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
    return 0;
}

/*
 * Finds the correct entry for a given path.
 */
entry* findDir(char **pathArr, entry* folder){
    size_t length = sizeof(pathArr) / sizeof(pathArr[0]);
    if(length == 2){
        return folder;
    }
    
    entry *currentEntry = folder;
    for(int pathI = 0; pathI < length; pathI++){
        for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
            entry *file = (entry *) currentEntry->data;
            if(file[fileI].type == TYPE_DIR && strcmp(file[fileI].name, pathArr[pathI]) == 0)
            {
                currentEntry = &file[fileI];
                fileI = DEFAULT_DIR_SIZE; // Next path
            }
        }
    }
    return currentEntry;
}

/*
 * @returns a array of char array
 */
char **splitString(const char *input, char delimiter){
    int count = 0;
    int j = 0;
    char **out = calloc(count, sizeof(char *));
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
            count++;
            j = i;
        }
    }
    out[count] = calloc(((strlen(input) - 1) - j), sizeof(char));

    int target = 0;
    for(int i = 0; i < strlen(input); i++)
    {
        if(input[i] == delimiter)
        {
            target++;
            continue;
        }
        sprintf(out[target], "%s%c", out[target], input[i]);
    }
    return out;
}