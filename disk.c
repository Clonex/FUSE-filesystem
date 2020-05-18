

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
            printf("File: %s, Index: %d, j: %d, Type: %d\n", currentFile.name, index, j, currentFile.type);
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
            printf("Memcpy()\n");
            memcpy(tempBlock, output, strlen(output) * sizeof(char));
            printf("strcat()\n");
            strcat(tempBlock, block);
            printf("free()\n");
            free(output);
            free(block);
            output = tempBlock;
            printf("Output: %s\n", output);
        }
    }
    printf("Done..\n");
    free(queue);
}

char *createBlock(entry file)
{
    printf("createBlock(): running..\n");
    char *type = malloc(sizeof(char) * 2);
    char *size = malloc(10 + 1);
    char *modTime = malloc(12 + 1);
    char *accessTime = malloc(12 + 1);
    printf("wau1\n");

    sprintf(type, "%d", file.type);
    sprintf(size, "%d", file.size);
    sprintf(modTime, "%ld", file.modTime);
    sprintf(accessTime, "%ld", file.accessTime);

    printf("wau2\n");
    char *temp[] = {
        pad(file.name, DEFAULT_NAME_SIZE, '/', false), 
        type,
        pad(size, 10, '0', true),
        pad(modTime, 12, '0', true),
        pad(accessTime, 12, '0', true)
    };
    
    printf("wau3\n");
    int headerSize = DEFAULT_NAME_SIZE + 2 + 11 + 13 + 13;
    char *out = malloc(headerSize + file.size);
    printf("wau4\n");
    if(out == NULL)
    {
        return NULL;
    }
    printf("Adding to new block\n");
    for(int i = 0; i < 5; i++)
    {
        printf("Added to block: %s\n", temp[i]);
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
    printf("pad()\n");
    int diff = (length - strlen(value));
    printf("pad(): diff: %d\n", diff);
    char *padString = malloc(diff);
    if(padString == NULL)
    {
        return NULL;
    }
    
    for(int i = 0; i < diff; i++)
    {
        memcpy(padString + i, &padding, 1);
    }
    printf("pad(): padString: %s\n", padString);

    printf("pad(): strlen(value): %ld, length: %d, diff: %d\n", strlen(value), length, diff);
    char *new = malloc(length);
    if(new == NULL)
    {
        return NULL;
    }
    printf("pad(): Malloced\n");

    if(leftPad)
    {
        memcpy(new, padString, diff);
        memcpy(new + diff, value, strlen(value));
    }else{ // Right pad
        memcpy(new, value, strlen(value));
        memcpy(new + strlen(value), padString, diff);
    }
    printf("pad(): free()\n"    );
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