#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/__init__.h"

Table *NewTable(String *name, String data) {
    Table *t = (Table *)malloc(sizeof(Table));
    *t = (Table) {
        .Name       = NewString(name->data),
        .Keys       = (char **)malloc(sizeof(char *) * 1),
        .KeyCount   = 0,
        .Rows       = (Array **)malloc(sizeof(Array *) * 1),

        .AddTable   = AppendKey,
        .Destruct   = DestroyTable
    };

    data.TrimAt(&data, 0);
    data.TrimAt(&data, data.idx);
    data.Trim(&data, ')');

    char **raw_args = data.Split(&data, "','");
    Array args = NewArray(NULL);
    args.Merge(&args, (void **)raw_args);

    for(int i = 0; i < args.idx; i++) {
        String arg = NewString(args.arr[i]);
        if(!strcmp(arg.data, ")"))
            continue;

        t->Keys[t->KeyCount] = strdup(arg.data);
        t->KeyCount++;
        t->Keys = (char **)realloc(t->Keys, sizeof(char *) * (t->KeyCount + 1));
    }

    return t;
}

void print_table(Table *t) {
    for(int i = 0; i < t->RowCount; i++) {
        for(int idx = 0; idx < t->Rows[i]->idx; idx++) {
            printf("%s", (char *)t->Rows[i]->arr[idx]);
        }
    }
}

int AppendKey(Table *t, const char *k) {
    if(!t)
        return 0;

    t->Keys[t->KeyCount] = strdup(k);
    t->KeyCount++;
    t->Keys = (char **)realloc(t->Keys, sizeof(char *) * (t->KeyCount + 1));
}

void DestroyTable(Table *t) {
    if(t->Name.data)
        t->Name.Destruct(&t->Name);

    if(t->KeyCount > 0) {
        for(int i = 0; i < t->KeyCount; i++)
            free(t->Keys[i]);

        free(t->Keys);
    }

    free(t);
}