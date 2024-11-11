#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/__init__.h"

int main(int argc, char *argv[]) {
    Database *db = InitDb("users");
    printf("Tables: %ld loaded.....!\n", db->TableCount);
    for(int i = 0; i < db->TableCount; i++) {
        printf("%s: \n", db->Tables[i]->Name);
        for(int col = 0; col < db->Tables[i]->KeyCount; col++)
            printf("%s ", db->Tables[i]->Keys[col]);
        printf("\n");
    }
    return 0;
}