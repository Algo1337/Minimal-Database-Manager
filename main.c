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
    // table->Get(table, "PASSWORD")->Where(table, "USERNAME")->Query(table, "jeffybob");
    
    // ('1','jeffybob','admin@chat.io','lulbob','1.1.1.1','3','4')
    table->Create(table, (const char *[]){"1", "jeffybob", "admin@chat.io", "lulbob", "1.1.1.1", "3", "4", NULL}); // Creates @ last row
    // table->Create(table, new_row_args....)->Where(table, "USERNAME")->Query(table, "jeffybob"); // Create after the row containing the username 'jeffybob'

    char *db_query = (char *)table->Execute(table);
    // printf("%s\n", db_query);

    for(int i = 0; i < db->TableCount; i++) {
        printf("%s: \n", db->Tables[i]->Name);

        for(int col = 0; col < db->Tables[i]->KeyCount; col++)
            printf("%s ", db->Tables[i]->Keys[col]);

        printf("\n");

        for(int val = 0; val < db->Tables[i]->RowCount; val++) {
            printf("%s\n", db->Tables[i]->Rows[val]);
        }

        printf("\r\n");
    }

    free(db_query);
    db->Destruct(db);
    return 0;
}