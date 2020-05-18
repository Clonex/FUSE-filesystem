int countFolders(entry *file);
char *createBlock(entry file);
char *pad(char *value, int length, char padding, bool leftPad);

void saveToDisk(entry *file)
{
    int folders = countFolders(file);
    printf("Total folders: %d\n", folders);
    int length = folders * sizeof(entry);
    entry *queue = malloc(length);
    if(queue == NULL)
    {
        return;
    }
    char *output = malloc(0);
    if(output == NULL)
    {
        return;
    }
    int inputIndex = 1;

    memcpy(queue, file, sizeof(entry));

    for(int index = 0; index < folders; index++){
        entry folder = queue[index];
        printf("---------\nFolder: %s\n", folder.name);
        entry *data = (entry *) folder.data;
        for(int j = 0; j < DEFAULT_DIR_SIZE; j++){
            entry currentFile = data[j];
            printf("File: %s, Type: %d\n", currentFile.name, currentFile.type);
            if(currentFile.type == TYPE_DIR)
            {
                printf("Adding to queue! inputIndex: %d\n", inputIndex);
                memcpy(&queue[inputIndex], &data[j], sizeof(entry));
                inputIndex++;
            }
            char *block = createBlock(currentFile);
            char *tempBlock = malloc((strlen(output) + strlen(block)) * sizeof(char));
            if(tempBlock == NULL)
            {
                return;
            }
            memcpy(tempBlock, output, strlen(output) * sizeof(char));
            strcat(tempBlock, block);
            free(output);
            output = tempBlock;
            printf("Output: %s\n", output);
        }
    }
}

char *createBlock(entry file)
{
    char *type;
    sprintf(type, "%d", file.type);
    char *size;
    sprintf(size, "%d", file.size);
    char *modTime;
    sprintf(modTime, "%ld", file.modTime);
    char *accessTime;
    sprintf(accessTime, "%ld", file.accessTime);

    char temp[] = {
        pad(file.name, DEFAULT_NAME_SIZE, '/', false), 
        pad(type, 1, '0', true),
        pad(size, 10, '0', true),
        pad(modTime, 12, '0', true),
        pad(accessTime, 12, '0', true)
    };
    
    int headerSize = DEFAULT_NAME_SIZE + 1 + 10 + 12 + 12;
    char *out = malloc(headerSize + file.size);
    if(out == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < getLength(temp); i++)
    {
        strcat(out, temp[i]);
        free(temp[i]);
    }

// 
    

    
    

    // strcat(out, name);
    // strcat(out, type);
    // strcat(out, size);
    // strcat(out, modTime);
    // strcat(out, accessTime);
    // strcat(out, file->data);

    // free(name);
    // free(size);
    // free(modTime);
    // free(accessTime);
    return out;
}

char *pad(char *value, int length, char padding, bool leftPad)
{
    int diff = (length - strlen(value));
    char *padString = malloc(diff);
    if(padString == NULL)
    {
        return;
    }
    
    for(int i = 0; i < diff; i++)
    {
        memcpy(padString + i, &padding, 1);
    }
    char *new = malloc(length);
    if(new == NULL)
    {
        return;
    }
    if(leftPad)
    {
        memcpy(new, padString, diff);
        memcpy(new + diff, value, strlen(value));
    }else{ // Right pad
        memcpy(new, value, strlen(value));
        memcpy(new + strlen(value), padString, diff);
    }
    free(padString);
    return new;
}

int countFolders(entry *file)
{
    int ret = 1;
    entry *data = (entry *) file->data;
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++){
        entry current = data[i];
        if(current.type == TYPE_DIR)
        {
            ret += countFolders(&current);
        }
    }
    return ret;
}