/*
 * Finds the file associated with the given file-path and saves the attributes of that file into the given struct stat
 */
int getAttributes(const char *path, struct stat *stbuf){
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp( path, "/" ) == 0){ // root folder
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}else{
		char **tempPath = splitString(path, '/', true);
		if(tempPath == NULL)
		{
			return -ENOMEM;
		}
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
 * Frees all contents of the given directory
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
 * Creates an entry of type type with the name specified in path. Adds the entry to the parent folder specified in path.
 */
int createEntry(const char *path, int type)
{
    entry *file = calloc(1, sizeof(entry));
    if(file == NULL){
        return -ENOMEM;
    }
    
    if(type == TYPE_DIR) // directories are filled with blank entries
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
    
    char **nameArr = splitString(path, '/', true);  // used for retrieving the name of the file
    char **pathArr = splitString(path, '/', false); // used to find the parent directory
    if(pathArr == NULL || nameArr == NULL)
    {
        return -ENOMEM;
    }

    file->name = nameArr[getLength(nameArr) - 1];
    if(strlen(file->name) > DEFAULT_NAME_SIZE) // filenames caps at 32 chars
    {
        return -ENAMETOOLONG; 
    }

    entry *targetDir = root_fs;
    if(getLength(pathArr) != 1) // if length of pathArr is 1, then the entry must be added to the root directory
    {
        targetDir = findEntry(pathArr);
    }
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++) // find the first blank entry in the parent folder and insert the entry in that spot
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
 * Returns the entry represented by pathArr. 
 */
entry* findEntry(char **pathArr)
{
    size_t length = getLength(pathArr);
    entry *currentEntry = root_fs;
    for(int pathI = 0; pathI < length; pathI++) // loop through the steps in the path
    {
        for(int fileI = 0; fileI < DEFAULT_DIR_SIZE; fileI++) // loop through the entries in the current directory
        {
            entry *data = (entry *) currentEntry->data;
            if(data[fileI].type != TYPE_BLANK && strcmp(data[fileI].name, pathArr[pathI]) == 0) // the current entry is the current step in the path
            {
                currentEntry = &data[fileI];
                break; // Next step in path
            }
        }
    }
    
    if(strcmp(currentEntry->name, pathArr[length - 1]) == 0) // the right entry was found
    {
        return currentEntry;
    }
    return NULL;
}

/*
 * Returns an array of the input splitted by the delimiter. includeLast determines whether or not the last entry in the path should be included or not.
 */
char **splitString(const char *input, char delimiter, bool includeLast){
    int count = 0;  // number of delimiters in the input
    int j = 0;      // used to hold position of last delimiter and to find distance between this and the current delimiter
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
 * Returns the length of the given array of strings
 */
int getLength(char **arr)
{
    int count = -1;
    while(arr[++count]){}
    return count;
}