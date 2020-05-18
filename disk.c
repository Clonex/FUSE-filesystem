char *removePadding(char *value, char padding, bool leftPad);

void restoreImage()
{
    // TODO: check if file exists
    printf("restoreImage(): starting..\n");
    char c;
    FILE *fp = fopen("data.img", "r");
    
    entry *newEntry = malloc(sizeof(entry));
    if(newEntry == NULL){
        return;
    }
    
    char *buffer = calloc(1, (DEFAULT_NAME_SIZE + 1));
    fgets(buffer, (DEFAULT_NAME_SIZE + 1), fp);
    newEntry->name = removePadding(buffer, '/', false);

    buffer = calloc(1, 11);
    fgets(buffer, 11, fp);
    newEntry->size = removePadding(buffer, '0', true);

    buffer = calloc(1, 13);
    fgets(buffer, 13, fp);
    newEntry->modTime = removePadding(buffer, '0', true);

    buffer = calloc(1, 13);
    fgets(buffer, 13, fp);
    newEntry->accessTime = removePadding(buffer, '0', true);

    buffer = calloc(1, 2);
    fgets(buffer, 2, fp);
    newEntry->type = (int) buffer;

    printf("File name: %s\n", newEntry->name);
    printf("File size: %s\n", newEntry->size);
    printf("File modTime: %s\n", newEntry->modTime);
    printf("File accessTime: %s\n", newEntry->accessTime);
    printf("File type: %s\n", newEntry->type);
    
    fclose(fp);
}

char *removePadding(char *value, char padding, bool leftPad)
{
    if(leftPad)
    {
       int i;
        for(i = strlen(value); i >= 0; i--)
        {
            if(value[i] == padding)
            {
                break;
            }
        }
        int length = strlen(value) - i;
        char *newBlock = calloc(1, length);
        memcpy(newBlock, value + i + 1, length);
        //strcat(newBlock, value + i);
        return newBlock;
    }
     int i;
    for(i = 0; i < strlen(value); i++)
    {
        if(value[i] == padding)
        {
            break;
        }
    }
    
    char *newBlock = calloc(1, i + 1);
    memcpy(newBlock, value, i);
    return newBlock;
    
}

void saveToDisk(entry *file)
{
    int folders = countFolders(file);
    int length = folders * sizeof(entry);
    entry *queue = malloc(length);
    if(queue == NULL)
    {
        return;
    }
    char *output = calloc(1, 1);
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

            char *tempBlock = calloc(1, (strlen(output) + 1) + (strlen(block) + 1));
            if(tempBlock == NULL)
            {
                return;
            }
            strcat(tempBlock, output);
            strcat(tempBlock, block);

            free(output);
            output = tempBlock;
            free(block);
        }
    }
    printf("\nContents: %s\n", output);
    free(queue);

    printf("SAving\n");
    FILE *fp = fopen("data.img", "w+");
    fprintf(fp, "%s", output); 
    fclose(fp);

    free(output);
}

char *createBlock(entry file)
{
    char *type = malloc(2);
    char *size = malloc(10 + 1);
    char *modTime = malloc(12 + 1);
    char *accessTime = malloc(12 + 1);

    sprintf(type, "%d", file.type);
    sprintf(size, "%d", file.size);
    sprintf(modTime, "%ld", file.modTime);
    sprintf(accessTime, "%ld", file.accessTime);
    
    int headerSize = (DEFAULT_NAME_SIZE + 1) + 2 + 11 + 13 + 13;
    
    char *out = calloc(1, (headerSize + file.size));
    if(out == NULL)
    {
        printf("Error\n");
        return NULL;
    }

    char *tempPad = pad(file.name, DEFAULT_NAME_SIZE, '/', false);
    strcat(out, tempPad);
    free(tempPad);

    tempPad = pad(size, 10, '0', true);
    strcat(out, tempPad);
    free(tempPad);

    tempPad = pad(modTime, 12, '0', true);
    strcat(out, tempPad);
    free(tempPad);

    tempPad = pad(accessTime, 12, '0', true);
    strcat(out, tempPad);
    free(tempPad);
    
    strcat(out, type); 

    if(file.data != NULL)
    {
        printf("DATA: %s\n", (char *)file.data);
        strcat(out, (char *)file.data); 
    }

    free(type);
    free(size);
    free(modTime);
    free(accessTime);

    return out;    
}

char *pad(char *value, int length, char padding, bool leftPad)
{
    int diff = (length - strlen(value));
    char *padString = calloc(1, diff);
    if(padString == NULL)
    {
        return NULL;
    }
    
    for(int i = 0; i < diff; i++)
    {
        memcpy(padString + i, &padding, 1);
    }

    char *new = calloc(length + 2, sizeof(char));
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