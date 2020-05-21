#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h> 
#include <time.h>


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

int getLength(char **arr)
{
    int count = -1;
    while(arr[++count]){}
    return count;
}

void main(){
    
    printf("createBlock(): running..\n");
    char *type = malloc(sizeof(char));
    char *size = malloc(10);
    char *modTime = malloc(12);
    char *accessTime = malloc(12);

    time_t stamp;
    time(&stamp);
    
    sprintf(type, "%d", 1);
    sprintf(size, "%d", 100);
    sprintf(modTime, "%ld", stamp);
    sprintf(accessTime, "%ld", stamp);

    char *temp[] = {
        pad("mappe", 32, '/', false), 
        pad(type, 1, '0', true),
        pad(size, 10, '0', true),
        pad(modTime, 12, '0', true),
        pad(accessTime, 12, '0', true)
    };

    int headerSize = 32 + 1 + 10 + 12 + 12;
    char *out = malloc(headerSize + 100);
    if(out == NULL)
    {
        return;
    }
    printf("Adding to new block, length = %d\n", strlen(temp));
    for(int i = 0; i < 5; i++)
    {
        printf("Adding to block: %s\n", temp[i]);
        strcat(out, temp[i]);
        free(temp[i]);
        
        printf("Added to block: %d\n", i);
    }
    printf("Huh\n");
    free(type);
    free(size);
    free(modTime);
    free(accessTime);
    
}