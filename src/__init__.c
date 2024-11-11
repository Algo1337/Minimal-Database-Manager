#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/__init__.h"

Database *InitDb(const char *db_name) {
    Database *db = (Database *)malloc(sizeof(Database));
    *db = (Database){
        .Name           = (char *)malloc(strlen(db_name) + 4),
        .Tables         = (Table **)malloc(sizeof(Table *) * 1),
        .TableCount     = 0
    };

    memset(db->Name, '\0', strlen(db_name) + 4);
    strcpy(db->Name, db_name);
    strcat(db->Name, ".db");
    
    db->Con = fopen(db->Name, "r+");
    if(!db->Con)
        return NULL;

    RetrieveDatabase(db);
    ParseTables(db);

    return db;
}

int RetrieveDatabase(Database *db) {
    fseek(db->Con, 0L, SEEK_END);
    long sz = ftell(db->Con);
    fseek(db->Con, 0L, SEEK_SET);

    db->Data = (char *)malloc(sz);
    memset(db->Data, '\0', sz);

    fread(db->Data, sz, 1, db->Con);
    return strlen(db->Data);
}

int ParseTables(Database *db) {
    String raw_data = NewString(db->Data);
    raw_data.Trim(&raw_data, '(');
    raw_data.Trim(&raw_data, ')');

    Array lines = NewArray(NULL);
    lines.Merge(&lines, (void **)raw_data.Split(&raw_data, "\n"));
    for(int i = 0; i < lines.idx; i++)
    {
        if(!lines.arr[i])
            break;

        String line = NewString(lines.arr[i]);
        Array args = NewArray(NULL);
        args.Merge(&args, (void **)line.Split(&line, "','"));
        args.arr[args.idx - 1] = NULL;
        
        if(line.isEmpty(&line))
            break;

        if(strstr(line.data, "[@TABLE")) {
            char *name = line.GetSubstr(&line, 9, line.idx - 1);
            Table *new = NewTable(name, (const char *)lines.arr[i + 1]);
            AppendTable(db, new);
            i++;
            line.Destruct(&line);
            args.Destruct(&args);
            continue;
        }

        if(db->TableCount > 0)
            printf("%ld => %ld\n", args.idx, db->Tables[db->TableCount - 1]->KeyCount);

        if(args.idx < 7) {
            printf("Corrupted Line.....!\r\n\t= > %s\n", line.data);
            line.Destruct(&line);
            args.Destruct(&args);
            continue;
        }

        line.Destruct(&line);
        args.Destruct(&args);
    }
}