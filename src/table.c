#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/table.h"

Table *NewTable(const char *name, const char *key_line) {
    Table *t = (Table *)malloc(sizeof(Table));
    *t = (Table){
        .Name       = strdup(name),
        .Keys       = (char **)malloc(sizeof(char *) * 1),
        .KeyCount   = 0,

        .Rows       = (char **)malloc(sizeof(char *) * 1),
        .RowCount   = 0,

        .Destruct   = DestroyTable
    };

    String keys = NewString(key_line);
    Array args = NewArray(NULL);
    args.Merge(&args, (void **)keys.Split(&keys, "','"));
    args.arr[args.idx] = NULL;

    for(int i = 0; i < args.idx; i++) {
        if(!args.arr[i])
            break;

        t->Keys[t->KeyCount] = strdup(args.arr[i]);
        t->KeyCount++;
        t->Keys = (char **)realloc(t->Keys, sizeof(char *) * (t->KeyCount + 1));
    }

    t->Keys[t->KeyCount] = NULL;

    args.Destruct(&args);
    keys.Destruct(&keys);
    return t;
}

void DestroyTable(Table *t) {
    if(t->Name)
        free(t->Name);

    if(t->Keys) {
        for(int i = 0; i < t->KeyCount; i++)
            if(t->Keys[i])
                free(t->Keys[i]);
    }

    if(t->Rows) {
        for(int i = 0; i < t->RowCount; i++)
            if(t->Rows[i])
                free(t->Rows[i]);
    }
}