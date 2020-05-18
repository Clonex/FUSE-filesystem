

void saveToDisk(entry *file)
{
    int folders = countFolders(file);
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
        entry *data = (entry *) folder.data;
        for(int j = 0; j < DEFAULT_DIR_SIZE; j++){
            entry currentFile = data[j];
            if(currentFile.type == TYPE_DIR)
            {
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
            free(block);
            output = tempBlock;
        }
    }
    free(queue);
    free(output);
}

char *createBlock(entry file)
{
    char *type = malloc(sizeof(char) * 2);
    char *size = malloc(10 + 1);
    char *modTime = malloc(12 + 1);
    char *accessTime = malloc(12 + 1);

    sprintf(type, "%d", file.type);
    sprintf(size, "%d", file.size);
    sprintf(modTime, "%ld", file.modTime);
    sprintf(accessTime, "%ld", file.accessTime);

    char *temp[] = {
        pad(file.name, DEFAULT_NAME_SIZE, '/', false), 
        type,
        pad(size, 10, '0', true),
        pad(modTime, 12, '0', true),
        pad(accessTime, 12, '0', true)
    };

    free(type);
    free(size);
    free(modTime);
    free(accessTime);

    
    int headerSize = DEFAULT_NAME_SIZE + 2 + 11 + 13 + 13;
    char *out = malloc(headerSize + file.size);
    if(out == NULL)
    {
        return NULL;
    }
    for(int i = 0; i < 5; i++)
    {
        strcat(out, temp[i]);
        if(i != 1)
        {
            free(temp[i]);
        }
    }

    return out;
}

char *pad(char *value, int length, char padding, bool leftPad)
{
    int diff = (length - strlen(value));
    char *padString = malloc(diff);
    if(padString == NULL)
    {
        return NULL;
    }
    
    for(int i = 0; i < diff; i++)
    {
        memcpy(padString + i, &padding, 1);
    }

    char *new = malloc(length);
    if(new == NULL)
    {
        return NULL;
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