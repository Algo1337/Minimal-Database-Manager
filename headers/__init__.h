#pragma once

#include <str.h>
#include <arr.h>

typedef struct Table {
    String  Name;

    char    **Keys;
    long    KeyCount;

    Array   **Rows;
    long    RowCount;

    int     (*AddTable)     (struct Table *t, const char *k);
    void    (*Destruct)     (struct Table *t);
} Table;

typedef struct HeinekenDB {
    String  Path;
    FILE    *Connection;
    String  Data;

    Table   **Tables;
    long    TableCount;
}  HeinekenDB;

HeinekenDB      *InitDatabase(const char *db_name);

int             RetrieveTables(HeinekenDB *db);
int             ParseDB(HeinekenDB *db);
int             ParseTable(HeinekenDB *db, Table *t, Array lines, int line_num);
void            print_table(Table *t);

Table           *NewTable(String *name, String data);
int             AppendKey(Table *t, const char *k);
void            DestroyTable(Table *t);