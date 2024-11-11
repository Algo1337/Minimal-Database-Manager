#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <str.h>
#include <arr.h>

#include "headers/__init__.h"
int main() {
    HeinekenDB *db = InitDatabase("users");
    printf("[ + ] %ld Tables Loaded....!\n", db->TableCount);

    for(int i = 0; i < db->TableCount; i++) {
        Table *cur = db->Tables[i];

        if(!cur)
            break;

        printf("Table: %s : %ld\r\n\t = > ", cur->Name.data, cur->KeyCount);
        for(int k = 0; k < cur->KeyCount; k++)
            printf("%s, ", cur->Keys[k]);

        printf("\n");

        print_table(cur);
            
        printf("\n");
    }
    return 0;
}