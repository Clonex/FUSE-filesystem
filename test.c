#define S_IFDIR 0
#define S_IFREG 0
struct stat {
    int st_mode;
    int st_nlink;
    int st_size;
    int st_atime;
    int st_mtime;
};

#include "lfs.h"
#include "implementation.c"
#include "disk.c"

/**
 * Creates root directory. Needed to execute various tests 
 */
void createRoot( void )
{
    root_fs = malloc(sizeof(struct entry));
	if(root_fs == NULL)
	{
		return; 
	}
	root_fs->name = "";
	root_fs->type = TYPE_DIR;

    root_fs->size = sizeof(entry) * DEFAULT_DIR_SIZE;
    root_fs->data = calloc(DEFAULT_DIR_SIZE, sizeof(entry));
    if(root_fs->data == NULL)
    {
        return; 
    }
    entry *data = (entry *) root_fs->data;
    for(int i = 0; i < DEFAULT_DIR_SIZE; i++)
    {
        data[i].type = TYPE_BLANK;
        data[i].name = "";
        data[i].size = 0;
    }
}

/**
 * Test for padding a string and removing padding
 */
void testPadding( void )
{
    char *testString = "We are testing padding";
    char *testPadLeft = pad( testString, 32, '/', true );
    char *testPadRight = pad( testString, 32, '/', false );
    char *expectedPadRight = "We are testing padding//////////";
    char *expectedPadLeft = "//////////We are testing padding";

    printf("String we are padding: \"%s\"\n", testString);
    printf("Expected result of padding right: \"%s\"\n", expectedPadRight);
    printf("Actual result: \"%s\"\n", testPadRight);
    printf("\n");

    printf("Expected result of padding left: \"%s\"\n", expectedPadLeft);
    printf("Actual result: \"%s\"\n", testPadLeft);
    printf("\n");
    
    printf("Removing padding from \"%s\"\n", testPadLeft);
    printf("Result: \"%s\"\n", removePadding( testPadLeft, '/', true ));
    printf("Removing padding from \"%s\"\n", testPadRight);
    printf("Result: \"%s\"\n", removePadding( testPadRight, '/', false ));
}

/**
 * Test for creating an entry
 */
void testCreateEntry( void )
{
    createEntry("/NewFile", TYPE_FILE);
    createEntry("/NewDir", TYPE_DIR);
    entry *root_data = (entry *) root_fs->data;
    entry newFile = root_data[0];
    entry newDir = root_data[1];

    printf("Created new file with name \"NewFile\" and directory with name \"NewDir\"\n");
    printf("Does the file have the correct name and type: \"%s\"\n", strcmp(newFile.name, "NewFile") == 0 && newFile.type == TYPE_FILE ? "Yes" : "No");
    printf("Does the directory have the correct name and type: \"%s\"\n", strcmp(newDir.name, "NewDir") == 0 && newDir.type == TYPE_DIR ? "Yes" : "No");
}

/**
 * Test for finding an entry
 */
void testFindEntry( void )
{
    createEntry("/FileToBeFound", TYPE_FILE);
    entry *file = findEntry(splitString("/FileToBeFound", '/', true));

    printf("We have made file named \"FileToBeFound\" and tried to find it\n");
    printf("Does the found file have the correct name and type: \"%s\"\n", strcmp(file->name, "FileToBeFound") == 0 && file->type == TYPE_FILE ? "Yes" : "No");
}

/**
 * Test for splitting a string
 */
void testSplitString( void )
{
    char *stringToBeSplit = "/Hello/There/How/Are/You";
    char **stringAfterSplit = splitString(stringToBeSplit, '/', true);

    printf("String to be split: \"%s\"\n", stringToBeSplit);

    printf("Expected first string: \"\"\n");
    printf("Result: \"%s\"\n", stringAfterSplit[0]);
    printf("Expected second string: \"Hello\"\n");
    printf("Result: \"%s\"\n", stringAfterSplit[1]);
    printf("Expected third string: \"There\"\n");
    printf("Result: \"%s\"\n", stringAfterSplit[2]);
    printf("Expected fourth string: \"How\"\n");
    printf("Result: \"%s\"\n", stringAfterSplit[3]);
    printf("Expected fifth string: \"Are\"\n");
    printf("Result: \"%s\"\n", stringAfterSplit[4]);
    printf("Expected fifth string: \"You\"\n");
    printf("Result: \"%s\"\n", stringAfterSplit[5]);
}

/**
 * Test for counting folders
 */
void testCountingFolders( void )
{
    createEntry("/NewFolder", TYPE_DIR);
    createEntry("/NewFolder2", TYPE_DIR);
    createEntry("/NewFolder3", TYPE_DIR);
    int folders = countFolders(root_fs);

    printf("We have created 3 additional folders\n");
    printf("Expected amount of folders is: 4\n");
    printf("Result: %d\n", folders);
}

int main( int argc, char **argv )
{
    createRoot();
    printf("----------------------------------------------------------------\n");
    int test = atoi(argv[1]);
    switch (test)
    {
        case 1:
            
            printf("We are testing padding\n");
            testPadding();
            break;
        
        case 2:

            printf("We are testing splitting a string\n");
            testSplitString();
            break;
        
        case 3:

            printf("We are testing creating a entry\n");
            testCreateEntry();
            break;

        case 4:

            printf("We are testing finding a entry\n");
            testFindEntry();
            break;

        case 5:

            printf("We are testing counting folders\n");
            testCountingFolders();
            break;

        default:

            printf("Invalid test\n");
            break;
    }

    return 0;
}