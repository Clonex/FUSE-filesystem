#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main(){
    
    char *name;

    name = "fil";
    printf("length = %ld\n", strlen(name));

   char *longString = "ksdfdlksfljklsdkjfjsalfæaslkjfhjklashdfælkjhdsafdlæhasdljkfhælasfhsæa";
   if(strlen(longString) > 32)
   {
       sprintf(name, "%.*s\0", 31, longString);
   }
    
    printf("length = %ld\n", strlen(name));
}