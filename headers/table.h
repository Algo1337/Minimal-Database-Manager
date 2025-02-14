#pragma once

#include <str.h>
#include <arr.h>

typedef struct Table {
    char            *Name;

    char            **Keys;
    long            KeyCount;

    char            **Rows;
    long            RowCount;

    char            *Search;
    char            *CreateRow;
    char            *SearchUnderColumn;
    char            *GrabColumn;

    struct Table    *(*Create)   (struct Table *t, const char **arr);
    struct Table    *(*Get)      (struct Table *t, const char *col);
    struct Table    *(*Where)    (struct Table *t, const char *col);
    struct Table    *(*Query)    (struct Table *t, const char *col);
    void            *(*Execute)  (struct Table *t);
    void            (*Destruct)  (struct Table *t, int del_keys, int del_rows);
} Table;

Table *NewTable(const char *name, const char *key_line);

int isColumnValid(Table *t);
Table *CreateRow(Table *t, const char **arr);

Table *GrabRowColumn(Table *t, const char *col);
Table *SearchUnder(Table *t, const char *col);
Table *SearchQuery(Table *t, const char *q);
void *ExecuteSearch(Table *t);

int AppendRow(Table *t, const char *row);
void DestroyTable(Table *t, int destroy_keys, int destroy_rows);