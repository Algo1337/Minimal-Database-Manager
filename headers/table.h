#pragma once

#include <str.h>
#include <arr.h>

typedef struct Table {
    char    *Name;

    char    **Keys;
    long    KeyCount;

    char    **Rows;
    long    RowCount;

    void    (*Destruct) (struct Table *t);
} Table;

Table *NewTable(const char *name, const char *key_line);
void DestroyTable(Table *t);