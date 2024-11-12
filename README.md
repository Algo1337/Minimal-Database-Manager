<div align="center">
    <h1>Minimal Database Manager</h1>
    <p>A database SDK for production ready applications</p>
</div>

# About
<p>A minimal database system similar to SQL-like query systems but in functions</p>

# How to Install
```
git clone https://github.com/Algo1337/Minimal-Database-Manager.git
cd Minimal-Database-Manager; make
```

# How to Use
```
Database *db = InitDb("users"); // Get a pipe stream for the database
Table *users_db = db->Select("users"); // Get a pipe stream for a table in the database

users_db->Get("PASSWORD")->Where("USERNAME")->Query("Jeffybob"); // set a query using functions

char *query = (char *)users_db.Execute(users_db);

printf("%s\n", query);
free(query);

users_db->Get("USERNAMES");

char **usernames = (char **)users_db.Execute(users_db);
```