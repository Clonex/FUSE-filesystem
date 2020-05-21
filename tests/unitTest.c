#include "src/lfs.h" 
#include "src/implementation.c" 
#include "src/disk.c" 

char successMsg[] = "\033[0;32mPASSED\033[0m";
char failedMsg[] = "\033[0;31mFAILED\033[0m";

void createRoot(){
    root_fs = malloc(sizeof(struct entry));
	if(root_fs == NULL)
	{
		return; // TODO error
	}
	root_fs->name = "";
	root_fs->type = TYPE_DIR;

    root_fs->size = sizeof(entry) * DEFAULT_DIR_SIZE;
    root_fs->data = calloc(DEFAULT_DIR_SIZE, sizeof(entry));
    if(root_fs->data == NULL)
    {
        return; // TODO: error
    }
    entry *data = (entry *) root_fs->data;
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        data[i].type = TYPE_BLANK;
        data[i].name = "";
        data[i].size = 0;
    }
}

void paddingTester()
{
    char *test = "Can this be padded?";
    char *out = pad(test, 32, '0', true);
    char *outRight = pad(test, 32, '0', false);

    out = removePadding(out, '0', true);
    outRight = removePadding(outRight, '0', false);
    bool passed = strcmp(out, test) == 0 && strcmp(outRight, test) == 0;

    printf("[Padding]\t%s\n", passed ? successMsg : failedMsg);
}

void folderCountTest()
{
    int count = countFolders(root_fs);
    bool passed = count == 3;
    printf("[countFolders]\t%s\n", passed ? successMsg : failedMsg);
}

void createEntryTest()
{
    createEntry("/weirdName", TYPE_FILE);
    entry *data = (entry *) root_fs->data;
    entry file = data[0];

    bool passed = strcmp(file.name, "weirdName") == 0;
    printf("[createEntry]\t%s\n", passed ? successMsg : failedMsg);
}
void findEntryTest()
{
    entry *file = findEntry(splitString("/weirdName", '/', true));

    bool passed = strcmp(file->name, "weirdName") == 0;
    printf("[findEntry]\t%s\n", passed ? successMsg : failedMsg);
}

void blockTest()
{
    entry *data = (entry *) root_fs->data;
    entry file = data[0];

    char *excpectedBlock = "test////////////////////////////00000004800015900584450015900584452";
    char *block = createBlock(file);

    bool passed = strcmp(excpectedBlock, block);
    printf("[Block]\t\t%s\n", passed ? successMsg : failedMsg);
}

void main()
{
    paddingTester();

    createRoot();
    createEntryTest();
    findEntryTest();

    createEntry("/test", TYPE_DIR);
    createEntry("/folder", TYPE_DIR);
    folderCountTest();

    blockTest();
}