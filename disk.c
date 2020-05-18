int countFolders(entry file);

void saveToDisk(entry file)
{
    int folders = countFolders(file);
    printf("Total folders: %d\n", folders);
    /*if(file->type == TYPE_DIR)
    {
        entry *data = (entry *) file->data;

    }else{

    }*/
}

int countFolders(entry file)
{
    int ret = 1;
    entry *data = (entry *) file->data;
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++){
        entry current = data[i];
        if(current.type == TYPE_DIR)
        {
            ret += countFolders(current);
        }
    }
    return ret;
}