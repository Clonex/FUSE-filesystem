#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <time.h>

int main(){
    FILE *fp = fopen("test123.txt", "r");
    if(!fp)
    {
        return 1;
    }
    char *buffer = calloc(1, 100);
    fgets(buffer, 10, fp);
    printf("results = %s\n", buffer);

    free(buffer);

    return 0;
}


