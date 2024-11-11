#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/__init__.h"

Table *FindTable(Database *db, const char *table_name) {
    if(!db)
        return NULL;

    for(int i = 0; i < db->TableCount; i++)
    {
        if(!db->Tables[i])
            break;

        if(!strcmp(db->Tables[i]->Name, table_name))
            return db->Tables[i];
    }

    return NULL;
}

int AppendTable(Database *db, Table *new) {
    if(!db || !new)
        return 0;

    if(!db->Tables)
        return 0;

    db->Tables[db->TableCount] = new;
    db->TableCount++;
    db->Tables = (Table **)realloc(db->Tables, sizeof(Table *) * (db->TableCount + 1));

    return 1;
}

int RemoveTable(Database *db, Table *rm) {
    if(!db || !rm)
        return 0;

    if(!db->Tables)
        return 0;

    Table **arr = (Table **)malloc(sizeof(Table *) * (db->TableCount));
    memset(arr, '\0', sizeof(Table *) * (db->TableCount));

    long idx = 0;
    for(int i = 0; i < db->TableCount; i++) {
        if(db->Tables[i] == rm) {
            db->Tables[i]->Destruct(db->Tables[i]);
            continue;
        }

        arr[idx] = db->Tables[i];
        idx++;
    }

    free(db->Tables);

    db->Tables = arr;
    db->TableCount = idx;

    return 1;
}