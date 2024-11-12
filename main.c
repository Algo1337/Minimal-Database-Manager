#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/__init__.h"

int main(int argc, char *argv[]) {
    Database *db = InitDb("users");
    printf("Tables: %ld loaded.....!\n", db->TableCount);

    for(int i = 0; i < db->TableCount; i++) {
        printf("%s: \n", db->Tables[i]->Name);

        for(int col = 0; col < db->Tables[i]->KeyCount; col++)
            printf("%s ", db->Tables[i]->Keys[col]);

        printf("\n\t = >");

        for(int val = 0; val < db->Tables[i]->RowCount; val++)
            printf("%s", db->Tables[i]->Rows[val]);

        printf("\r\n");
    }

    Table *table = db->Select(db, "USERS", table);
    table->Get(table, "PASSWORD")->Where(table, "USERNAME")->Query(table, "jeffybob");

    char *db_query = (char *)table->Execute(table);
    printf("%s\n", db_query);

    free(db_query);
    db->Destruct(db);
    return 0;
}