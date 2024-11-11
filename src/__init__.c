#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/__init__.h"

HeinekenDB *InitDatabase(const char *db_name) {
    HeinekenDB *db = (HeinekenDB *)malloc(sizeof(HeinekenDB));
    *db = (HeinekenDB){
        .Path = NewString(db_name),
        .Tables = (Table **)malloc(sizeof(Table *) * 1),
        .TableCount = 0
    };

    db->Path.AppendString(&db->Path, ".db");
    RetrieveTables(db);
    ParseDB(db);
    
    return db;
}

int ParseDB(HeinekenDB *db) {
    if(!db)
        return 0;

    char **lines_arr = db->Data.Split(&db->Data, "\n");
    Array lines = NewArray(NULL);
    lines.Merge(&lines, (void **)lines_arr);

    for(int i = 0; i < lines.idx; i++) {
        String line = NewString(lines.arr[i]);
        if(line.isEmpty(&line) || i == lines.idx - 1) {
            line.Destruct(&line);
            continue;
        }

        if(strstr(line.data, "[@TABLE")) {
            String next_line = NewString(lines.arr[i + 1]);

            line.TrimAt(&line, line.idx - 1);
            char *table_name = line.GetSubstr(&line, 9, line.idx - 1);
            String tble = NewString(table_name);

            db->Tables[db->TableCount] = NewTable(&tble, next_line);
            db->TableCount++;
            db->Tables = (Table **)realloc(db->Tables, sizeof(Table *) * (db->TableCount + 1));

            ParseTable(db, db->Tables[db->TableCount - 1], lines, i);

            next_line.Destruct(&next_line);
            tble.Destruct(&tble);
        }

        line.Destruct(&line);
    }

    db->Tables[db->TableCount] = NULL;

    lines.Destruct(&lines);
    return 1;
}

int ParseTable(HeinekenDB *db, Table *t, Array lines, int line_num) {
    for(int i = line_num; i < lines.idx; i++) {
        String line = NewString(lines.arr[i]);
        if(line.isEmpty(&line))
            break;

        line.TrimAt(&line, 0);
        line.TrimAt(&line, line.idx);
        line.Trim(&line, ')');

        char **args_arr = line.Split(&line, "','");
        Array args = NewArray(NULL);
        args.Merge(&args, (void **)args_arr);

        if(args.idx != t->KeyCount) {
            printf("[ x ] Corrupted Line......\r\n\t = > %s\r\n", line.data);
            line.Destruct(&line);
            args.Destruct(&args);
            continue;
        }

        t->Rows[t->RowCount] = (Array *)malloc(sizeof(Array));
        t->Rows[t->RowCount] = &args;

        t->RowCount++;
        t->Rows = (Array **)realloc(t->Rows, sizeof(Array *) * (t->RowCount + 1));
        
        line.Destruct(&line);
    }
}

int RetrieveTables(HeinekenDB *db) {
    if(!db)
        return 0;

    db->Connection = fopen(db->Path.data, "r+");
    if(!db->Connection)
        return 0;

    fseek(db->Connection, 0L, SEEK_END);
    long sz = ftell(db->Connection);
    fseek(db->Connection, 0L, SEEK_SET);

    char *Data = (char *)malloc(sz);
    if(!Data)
        return 0;

    memset(Data, '\0', sz);

    fread(Data, sz, 1, db->Connection);
    fclose(db->Connection);

    db->Data = NewString(Data);
    free(Data);

    return 1;
}