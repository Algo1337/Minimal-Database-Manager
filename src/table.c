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
        .CreateRow  = NULL,

        .Create     = CreateRow,
        .Get        = GrabRowColumn,
        .Where      = SearchUnder,
        .Query      = SearchQuery,
        .Execute    = ExecuteSearch,
        .Destruct   = DestroyTable
    };

    String keys = NewString(key_line);
    Array args = NewArray(NULL);
    args.Merge(&args, (void **)keys.Split(&keys, "','"));

    t->Keys = args.arr;
    t->KeyCount = args.idx;

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

Table *CreateRow(Table *t, const char **arr) {
    if(!t || !arr)
        return NULL;

    Array a = NewArray(NULL);
    a.Merge(&a, (void **)arr);

    if(t->KeyCount != a.idx) {
        printf("[ - ] Warning, Invalid key count....!\n");
        a.Destruct(&a);
        return NULL;
    }

    t->CreateRow = (char *)malloc(1);
    int idx = 0;
    int i = 0;

    t->CreateRow[0] = '(';
    t->CreateRow[1] = '\'';
    while(arr[i] != NULL) {
        idx += strlen(arr[i]) + 3;

        t->CreateRow = realloc(t->CreateRow, idx);
        if(!t->CreateRow)
            printf("HERE\n");

        strcat(t->CreateRow, arr[i]);
        if(idx == t->KeyCount) {
            strcat(t->CreateRow, "')");
            break;
        }

        strcat(t->CreateRow, "','");
        i++;
    }

    return t;
}

int isColumnValid(Table *t) {
    if(!t)
        return -1;

    int x = -1;
    // Get() find key
    for(int i = 0; i < t->KeyCount; i++) {
        if(!t->Keys[i])
            break;

        if(!strcmp(t->Keys[i], t->SearchUnderColumn))
            return 1;
    }

    return -1;
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
    // Set
    // Create()
    if(t->CreateRow != NULL) {
        if(t->SearchUnderColumn != NULL) {
            //insert row after a row
        } 

        t->Rows[t->RowCount] = t->CreateRow;
        t->RowCount++;
        t->Rows = (char **)realloc(t->Rows, sizeof(char *) * (t->RowCount + 1));
        if(!t->Rows)
            return NULL;

        return 1;
    }

    int x = isColumnValid(t);

    // Get
    // Query() and Where()
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

        t->Search = NULL;
        t->SearchUnderColumn = NULL;
        t->GrabColumn = NULL;
        t->CreateRow = NULL;
        return 1;
    }

    /* Get() only. Return all rows of the specified column */
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
    t->Search = NULL;
    t->SearchUnderColumn = NULL;
    t->GrabColumn = NULL;
    t->CreateRow = NULL;

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

void DestroyTable(Table *t, int destroy_keys, int destroy_rows) {
    if(t->Name)
        free(t->Name);

    if(destroy_keys && t->Keys) {
        for(int i = 0; i < t->KeyCount; i++)
            if(t->Keys[i])
                free(t->Keys[i]);
    }

    if(destroy_rows && t->Rows) {
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