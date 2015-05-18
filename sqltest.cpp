#include <iostream>
#include <sqlite3.h> //requires libsqlite3-dev installed
#include <stdio.h>
static int callbacktrack(void *data, int argc, char **argv, char**columnNames)
{
    (*(int*)data)++;
    return 0;
}
/*static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   //fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
    *(int *) data = 1;
   return 1;
}*/

int main()
{
    std::cout << "lelelel";
    sqlite3 *db;
    char *error = 0;
    char* data = "callback called\n";
    //maximum size of wikipedia article is 256bytes, so max size 300 should be good
    int maxSize = 300;
    int rc;
    rc = sqlite3_open("hddWikipedia/adj.db", &db);
    if(!rc)
    {
        std::cout << "opened db\n";
    }
    
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Titles (id INTEGER PRIMARY KEY, title TEXT COLLATE NOCASE)", NULL, NULL, &error);
    std::cout << "made it this far";
    
    sqlite3_stmt *stmt;
    std::string title = "le le le le pinis";
    std::string statement = "INSERT INTO Titles VALUES (NULL, @Title)";
    sqlite3_prepare_v2(db, statement.c_str(), maxSize, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_clear_bindings(stmt);
    sqlite3_reset(stmt);
    
    //test select stuff
    std::cout << "testing select \n";
    statement = "SELECT * FROM Titles WHERE Title = 'le le le le pinis'";
    int callBackCalled = 0;
    rc = sqlite3_exec(db, statement.c_str(), callbacktrack, (void*) &callBackCalled, &error);
    std::cout << "\nvalue of callbackcalled is" << callBackCalled << "\n";
    //sqlite3_exec(db, statement.c_str(), NULL, NULL, &error);
    //std::cout << *error << "\n";
    //sqlite3_prepare_v2( 

}
