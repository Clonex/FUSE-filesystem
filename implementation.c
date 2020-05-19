

/*
 * 
 */
int getAttributes(const char *path, struct stat *stbuf){
	memset(stbuf, 0, sizeof(struct stat));
	if( strcmp( path, "/" ) == 0 ) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else {
		char **tempPath = splitString(path, '/', true);
		if(tempPath == NULL)
		{
			return -ENOMEM; // TODO
		}
		int length = getLength(tempPath);
		char *fileName = tempPath[length - 1];
		entry *file = findEntry(tempPath);
		if(file != NULL)
		{
			if(file->type == TYPE_DIR)
			{
				stbuf->st_mode = S_IFDIR | 0755;
			}else{
				stbuf->st_mode = S_IFREG | 0755;
			}
			stbuf->st_nlink = 1;
			stbuf->st_size = file->size;
			stbuf->st_mtime = file->modTime;
			stbuf->st_atime = file->accessTime;
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
            recursiveRemoveDir(file);
        }
    }
    free(data);
}

/*
 * Adds a entry to the correct folder.
 */
int createEntry(const char *path, int type)
{
    entry *file = calloc(1, sizeof(entry));
    if(file == NULL){
        return -ENOMEM;
    }
    
    if(type == TYPE_DIR)
    {
        file->size = sizeof(entry) * DEFAULT_DIR_SIZE;
        file->data = calloc(DEFAULT_DIR_SIZE, sizeof(entry));
        if(file->data == NULL)
        {
            return -ENOMEM;
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
    file->accessTime = stamp;
    file->modTime = stamp;
    file->type = type;
    
    char **nameArr = splitString(path, '/', true);
    char **pathArr = splitString(path, '/', false);
    if(pathArr == NULL || nameArr == NULL)
    {
        return -ENOMEM;
    }

    file->name = nameArr[getLength(nameArr) - 1];
    if(strlen(file->name) > DEFAULT_NAME_SIZE)
    {
        return -ENAMETOOLONG; 
    }

    entry *targetDir = root_fs;
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
            return 0;
        }
    }
    return -ENFILE;
}

/*
 *
 */
void cutName(char* source, char *target)
{
    if(strlen(source) > DEFAULT_NAME_SIZE){
        memcpy( target, source, DEFAULT_NAME_SIZE );
        target[DEFAULT_NAME_SIZE] = '\0';
    }else{
        memcpy( target, source, strlen(source) );
        target[strlen(source)] = '\0';
    }
}

/*
 * Finds the correct entry for a given path.
 */
entry* findEntry(char **pathArr){
    size_t length = getLength(pathArr);
    entry *currentEntry = root_fs;
    for(int pathI = 0; pathI < length; pathI++){
        for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++){
            entry *file = (entry *) currentEntry->data;
            if(file[fileI].type != TYPE_BLANK && strcmp(file[fileI].name, pathArr[pathI]) == 0){
                currentEntry = &file[fileI];
                break; // Next path
            }
        }
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