

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
        for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
        {
            entry *temp = (entry *) file->data[i];
            temp->type = TYPE_BLANK;
        }
    }else{
        file->size = 0;
    }
    time_t stamp;
    time(&stamp);
    file->time = stamp;
    file->type = type;

    char *name = strtok(path, "/");
    size_t length = sizeof(name) / sizeof(name[0]);
    file->name = name[length - 1]; 
    file->access = ACCESS_READ_WRITE;

    entry *targetDir = findDir(name, root_fs);
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        entry *temp = (entry *) targetDir->data[i];
        if(temp->type == TYPE_BLANK)
        {
            ((entry *) targetDir->data[i]) = file;
            //temp = file;
            break;
        }
    }
    return 0;
}

entry* findDir(char *pathArr, entry* folder){
    size_t length = sizeof(pathArr) / sizeof(pathArr[0]);
    if(length == 2){
        return folder;
    }

    entry *currentEntry = folder;
    for(int pathI = 0; pathI < length; pathI++){
        for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
            entry *file = (entry *) currentEntry->data;
            if(strcomp(file[fileI]->name, pathArr[pathI]) == 0)
            {
                currentEntry = file[fileI];
            }
        }
    }
    return currentEntry;
}