/*
 * Checks whether the data.img file exists, and restores tile root_fs from it.
 */
void restoreFromDisk()
{
    printf("restoreFromDisk(): starting..\n");
    FILE *fp = fopen("data.img", "r");
    if(!fp)
    {
        return;
    }

    entry *queue = calloc(1, sizeof(entry));
    if(queue == NULL)
    {
        return;
    }
    
    int inputIndex = 1;
    int queueLength = 1;
    memcpy(queue, root_fs, sizeof(entry));
    int index = 0;

    while(index < queueLength){
        entry folder = queue[index];
        entry *data = (entry *) folder.data;

        for(int j = 0; j < DEFAULT_DIR_SIZE; j++){
            printf("j: %d\n", j);
            entry *newEntry = malloc(sizeof(entry));
            if(newEntry == NULL){
                return;
            }
            
            char *buffer = calloc(1, (DEFAULT_NAME_SIZE + 1));
            if(fgets(buffer, (DEFAULT_NAME_SIZE + 1), fp) == NULL)
            {
                return;
            }
            printf("name = %s\n", buffer);
            newEntry->name = removePadding(buffer, '/', false);

            buffer = calloc(1, 11);
            if(fgets(buffer, 11, fp) == NULL)
            {
                return;
            }
            printf("size = %s\n", buffer);
            newEntry->size = atoi(buffer);

            buffer = calloc(1, 13);
            if(fgets(buffer, 13, fp) == NULL)
            {
                return;
            }
            printf("modTime = %s\n", buffer);
            newEntry->modTime = atoi(buffer);

            buffer = calloc(1, 13);
            if(fgets(buffer, 13, fp) == NULL)
            {
                return;
            }
            printf("accessTime = %s\n", buffer);
            newEntry->accessTime = atoi(buffer);

            buffer = calloc(1, 2);
            if(fgets(buffer, 2, fp) == NULL)
            {
                return;
            }
            printf("type = %s\n", buffer);
            newEntry->type = atoi(buffer);

            free(buffer);

            if(newEntry->type == TYPE_FILE)
            {
                printf("file name = %s, file size = %d\n", newEntry->name, newEntry->size);
                newEntry->data = calloc(1, newEntry->size);
                if(fgets(newEntry->data, newEntry->size, fp) == NULL){
                    return;
                }
                printf("File data: '%s'\n", newEntry->data);
            }else if(newEntry->type == TYPE_DIR)
            {
                newEntry->size = sizeof(entry) * DEFAULT_DIR_SIZE;
                newEntry->data = calloc(DEFAULT_DIR_SIZE, sizeof(entry));
                if(newEntry->data == NULL)
                {
                    return; // ERROR
                }

                queueLength++;
                queue = realloc(queue, sizeof(entry) * queueLength);
                memcpy(&queue[inputIndex], newEntry, sizeof(entry));
                inputIndex++;
            }

            memcpy(&data[j], newEntry, sizeof(entry));
            printf("__________________ NEW ENTRY _________________\n");
        }
        index++;
    }
    free(queue);
    fclose(fp);
}

/*
 * Removes padding from a given string.
 */
char *removePadding(char *value, char padding, bool leftPad)
{
    if(leftPad)
    {
       int i;
        for(i = 0; i < strlen(value); i++)
        {
            if(value[i] != padding)
            {
                break;
            }
        }
        int length = strlen(value) - i;
        char *newBlock = calloc(1, length);
        memcpy(newBlock, value + i, length);
        return newBlock;
    }
     int i;
    for(i = strlen(value) - 1; i > 0; i--)
    {
        if(value[i] != padding)
        {
            break;
        }
    }
    char *newBlock = calloc(1, i + 1);
    if(i != 0)
    {
        memcpy(newBlock, value, i + 1);
    }
    return newBlock;
    
}

/*
 * Converts the @param file to a string and writes it to disk.
 */
void saveToDisk(entry *file)
{
    int folders = countFolders(file);
    int length = folders * sizeof(entry);
    entry *queue = malloc(length); // Contains the next
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
    free(queue);

    FILE *fp = fopen("data.img", "w+");
    fprintf(fp, "%s", output); 
    fclose(fp);

    free(output);
}

/*
 * Converts a entry into a string representation.
 */
char *createBlock(entry file)
{
    char *type = malloc(2);
    char *size = malloc(10 + 1);
    char *modTime = malloc(12 + 1);
    char *accessTime = malloc(12 + 1);

    int tempSize = 0;
    if(file.data)
    {
        tempSize = file.size;
    }

    // Converts the values into strings.
    sprintf(type, "%d", file.type);
    sprintf(size, "%d", tempSize);
    sprintf(modTime, "%ld", file.modTime);
    sprintf(accessTime, "%ld", file.accessTime);
    
    int headerSize = (DEFAULT_NAME_SIZE + 1) + 2 + 11 + 13 + 13;
    
    char *out = calloc(1, (headerSize + tempSize));
    if(out == NULL)
    {
        return NULL;
    }

    // Pads the strings, and adds them to the output string.
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

    if(file.data != NULL && file.type == TYPE_FILE)
    {
        strcat(out, (char *)file.data); 
    }

    free(type);
    free(size);
    free(modTime);
    free(accessTime);

    return out;    
}

/*
 * Pads the given string with the given char.
 */
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

/*
 * Counts the total folders for a given entry.
 */
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