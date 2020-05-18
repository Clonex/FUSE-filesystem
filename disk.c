int countFolders(entry *file);
char *createBlock(entry file);

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

    for(int index = 0; index < length; index++){
        entry folder = queue[index];
        printf("---------\nFolder: %s\n", folder.name);
        entry *data = (entry *) folder.data;
        for(int j = 0; j < DEFAULT_DIR_SIZE; j++){
            entry currentFile = data[j];
            printf("File: %s, Type: %d\n", currentFile.name, currentFile.type);
            if(currentFile.type == TYPE_DIR)
            {
                printf("Adding to queue! inputIndex: %d\n", inputIndex);
                memcpy(queue + (inputIndex * sizeof(entry)), &currentFile, sizeof(entry));
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
    return file.name;
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