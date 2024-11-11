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

        .Get        = GrabRowColumn,
        .Where      = SearchUnder,
        .Query      = SearchQuery,
        .Execute    = ExecuteSearch,
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

// Get
Table *GrabRowColumn(Table *t, const char *col) {
    if(!t)
        return NULL;

    for(int i = 0; i < t->KeyCount; i++) {
        if(!t->Keys[i])
            break;

        if(!strcmp(t->Keys[i], col)) {
            t->GrabColumn = strdup(col);
            return t;
        }
    }

    return t;
}

// Where
Table *SearchUnder(Table *t, const char *col) {
    if(!t || !t->Keys)
        return NULL;

    for(int i = 0; i < t->KeyCount; i++) {
        if(!t->Keys[i])
            break;

        if(!strcmp(t->Keys[i], col)) {
            t->SearchUnderColumn = strdup(col);
            return t;
        }
    }

    return t;
}

// Query
Table *SearchQuery(Table *t, const char *q) {
    if(!t)
        return NULL;

    t->Search = strdup(q);
    return t;
}

void *ExecuteSearch(Table *t) { 
    int x = -1;
    for(int i = 0; i < t->KeyCount; i++) {
        if(!strcmp(t->Keys[i], t->SearchUnderColumn)) {
            x = i;
            break;
        }
    }

    if(t->Search && t->SearchUnderColumn) {
        int y = -1;
        for(int i = 0; i < t->KeyCount; i++) {
            if(!strcmp(t->Keys[i], t->GrabColumn)) {
                y = i;
                break;
            }
        }

        if(x == -1 || y == -1)
            return NULL;

        for(int i = 0; i < t->RowCount; i++) {
            String row = NewString(t->Rows[i]);
            Array line_args = NewArray(NULL);
            line_args.Merge(&line_args, (void **)row.Split(&row, "','"));
            line_args.arr[line_args.idx] = NULL;

            if(!strcmp(line_args.arr[x], t->Search)) {
                char *val = strdup((char *)line_args.arr[y]);
                row.Destruct(&row);
                line_args.Destruct(&line_args);
                return val;
            }

            row.Destruct(&row);
            line_args.Destruct(&line_args);
        }
        return NULL;
    }

    if(x == -1)
        return NULL;

    char **arr = (char **)malloc(sizeof(char *) * t->RowCount);
    memset(arr, '\0', sizeof(char *) * (t->RowCount));
    long idx = 0;

    for(int i = 0; i < t->RowCount; i++) {
        String row = NewString(t->Rows[i]);
        Array args = NewArray(NULL);
        args.Merge(&args, (void **)row.Split(&row, "','"));

        arr[idx] = args.arr[x];
        idx++;

        row.Destruct(&row);
        args.Destruct(&args);
    }
    arr[idx] = NULL;
    if(idx > 0)
        return arr;

    return NULL;
}

int AppendRow(Table *t, const char *row) {
    if(!t || !row)
        return 0;

    if(!t->Rows)
        return 0;

    t->Rows[t->RowCount] = strdup(row);
    t->RowCount++;
    t->Rows = (char **)realloc(t->Rows, sizeof(char *) * (t->RowCount + 1));
    return 1;
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

    if(t->Search)
        free(t->Search);

    if(t->SearchUnderColumn)
        free(t->SearchUnderColumn);

    if(t->GrabColumn)
        free(t->GrabColumn);
        
    free(t);
}