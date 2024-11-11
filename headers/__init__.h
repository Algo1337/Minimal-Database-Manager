#pragma once

#include <str.h>
#include <arr.h>

#include "table.h"

typedef struct Database {
    char    *Name;

    Table   **Tables;
    long    TableCount;

    char    *Data;
    FILE    *Con;
} Database;

Database    *InitDb(const char *db_name);
int         RetrieveDatabase(Database *db);
int         ParseTables(Database *db);

// == [ MANAGER ] ==

Table *FindTable(Database *db, const char *table_name);
int AppendTable(Database *db, Table *new);
int RemoveTable(Database *db, Table *rm);