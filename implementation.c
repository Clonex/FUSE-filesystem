entry* findEntry(char **pathArr);
char **splitString(const char *input, char delimiter, bool includeLast);
int getLength(char **arr);

/*
 * 
 */
int getAttributes(const char *path, struct stat *stbuf){
    printf("getattr(): (path=%s)\n", path);

	memset(stbuf, 0, sizeof(struct stat));
	if( strcmp( path, "/" ) == 0 ) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else {
		char **tempPath = splitString(path, '/', true);
		if(tempPath == NULL)
		{
			return -1; // TODO
		}
		int length = getLength(tempPath);
		char *fileName = tempPath[length - 1];
		printf("getattr(): tempPath.length = %d\n", length);
		printArr(tempPath);
		
		printf("getattr(): Searching for file: %s\n", fileName);
		entry *file = findEntry(tempPath);
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
	}

	return 0;
}


/*
 * 
 */
void recursiveRemoveDir(entry* dir)
{
    entry *data = (entry *) dir->data;
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        entry *file = &data[i];
        if(file->type == TYPE_DIR)
        {
            recursiveRemoveDir(&file);
        }else if(file->type == TYPE_FILE){
            free(file->data);
        }
        free(file);
    }
    free(data);
}

/*
 * Adds a entry to the correct folder.
 */
int createEntry(const char *path, int type)
{
    printf("createEntry(): Creating file..\n");
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
        file->data = NULL;
    }
    time_t stamp;
    time(&stamp);
    file->time = stamp;
    file->type = type;
    
    char **nameArr = splitString(path, '/', true);
    char **pathArr = splitString(path, '/', false);
    if(pathArr == NULL || nameArr == NULL)
    {
        return -1; // TODO: error
    }
    
    file->name = nameArr[getLength(nameArr) - 1]; 
    file->access = ACCESS_READ_WRITE;

    entry *targetDir = root_fs;
    printArr(pathArr);
    printf("length %d\n", getLength(pathArr));
    if(getLength(pathArr) != 1)
    {
        targetDir = findEntry(pathArr);
    }
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        entry *data = (entry *) targetDir->data;
        if(data[i].type == TYPE_BLANK)
        {
            data[i] = *file;
            break;
        }
    }
    printf("createEntry(): Entry added..\n");
    return 0;
}

/*
 * Finds the correct entry for a given path.
 */
entry* findEntry(char **pathArr){
    size_t length = getLength(pathArr);
    printf("findEntry(): Checking dir..\n");
    entry *currentEntry = root_fs;
    for(int pathI = 0; pathI < length; pathI++){ // Måske (length - 1)
        printf("findEntry(): Current dir: %s\n", currentEntry->name);
        printf("findEntry(): Current path: %s\n", pathArr[pathI]);

        for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
            entry *file = (entry *) currentEntry->data;
            if(file[fileI].type != TYPE_BLANK && strcmp(file[fileI].name, pathArr[pathI]) == 0){
                currentEntry = &file[fileI];
                printf("findEntry(): New folder: %s\n", currentEntry->name);
                break; // Next path
            }
        }
        printf("\n");
    }
    
    if(strcmp(currentEntry->name, pathArr[length - 1]) == 0)
    {
        return currentEntry;
    }
    return NULL;
}

/*
 * @returns a array of the input splitted by the delimiter.
 */
char **splitString(const char *input, char delimiter, bool includeLast){
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
    if(includeLast)
    {
        out[count] = calloc(((strlen(input) - 1) - j), sizeof(char));
        memcpy(out[count], &input[j], (strlen(input) - j));
    }

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