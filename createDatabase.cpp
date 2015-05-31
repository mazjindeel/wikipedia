/* Author: Mazin Jindeel
** Date: 5/15
** Functionality: This utility is designed to remove namespaces from parsed Wikipedia data. 
** Usage: To run this utility, you must provide a minimum of one argument: the original, parsed file (with namespaces). However, there are also some switches you can use to configure the program. 
For example, a basic usage of this utility with no optional switches applied would look like this: 
./createDatabase parsedInputFile.txt 
Namespaces: If you wish to override the default namespaces.txt file, it is recommended that you merely add your namespaces to that file. Make sure to update the namespace counter at the top. The provided file removes most main namespaces, as of May 2015. However, if you want to overwrite the file, provide the switch 
-n fileNameSpaces.txt  
Forbidden Pages: This utility also logs any records that are not being included in the final output in a separate file. Logging is enabled by default. On slower filesystems or systems, it may be in your best interest to disable this logging feature. To do this, use the following:
-l true/false 
A value of "true" will enable logging, "false" will disable it

You can also specify a path to the log file with the following switch:
-log logfile.txt

An Sqlite Database will be created by this utility. You can specify a path to the database file, or it will default to adj.db. Additionally, if your hardware supports it, an argument of :memory will create the database entirely in main memory without writing to disk, which will greatly improve performance of this program. To specify the database path, use the following switch:
-db database.db 
*/ 
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sqlite3.h> //requries libsqlite3-dev installed

//prototypes
std::string parseLink(std::string line);
std::string parseTitle(std::string line);
bool isTitle(std::string line);
bool checkForbidden(std::string line, std::string *namespaces, int namespaceCount);
//return id if exists, enter to table and return id if not
int getTitleId(sqlite3 *adjDb, sqlite3_stmt *insertTitle, sqlite3_stmt *titleQuery, std::string line, int *existsCounter);
//do nothing if exists, else enter to table
int getLinkId(sqlite3 *adjDb, sqlite3_stmt *insertLink, sqlite3_stmt *linkQuery, int currentId, int toId, int *existsCounter);

int main(int argc, char* argv[])
{
    //default values
    //paths for all output files
    std::string parsedInputFile = "wikiParsed.txt";
    std::string outputFile = "wikiFullyParsed.txt"; 
    outputFile = "hddWikipedia/wikiFullyParsed.txt"; 
    std::string forbiddenFile = "forbiddenPages.txt"; 
    forbiddenFile = "hddWikipedia/forbiddenPages.txt";
    std::string namespaceFile = "namespaces.txt";
    std::string dbFile = "hddWikipedia/adj.db";
    bool log = true;

    //deal with command line args
    if(argc > 2) //if there was an input/output file specified at least
    {
        parsedInputFile = argv[1];
        for(int i = 1; i < argc; i++) //iterate through remaining arguments
        {
            if(strcmp(argv[i], "-l") == 0)
            {
                //if argument is "false", set log to false, otherwise set to true
                log = strcmp(argv[i+1], "false") != 0;       
                i++; //skip next item
            } 
            else if(strcmp(argv[i], "-log") == 0)
            {
                forbiddenFile = argv[i+1];
                i++;
            }
            else if(strcmp(argv[i], "-n") == 0)
            {
                namespaceFile = argv[i+1];
                i++;
            }
            else if(strcmp(argv[i], "-db") == 0)
            {
                dbFile = argv[i+1];
                i++;
            }
            
        }
    }
    //declare file streams
    std::ifstream reader;
    std::ofstream writer;
    std::ofstream forbiddenWriter;
    forbiddenWriter.open(forbiddenFile);
    //declare sql stuff
    sqlite3 *adjDb;
    sqlite3_stmt *insertTitle, *insertLink, *linkQuery, *titleQuery;
    char *sErrMsg = 0;
    int maxSize = 300; //wiki title 256 max, query is <50 bytes
    //initialize db
    sqlite3_open(dbFile.c_str(), &adjDb); //open db at file
    //create tables
    sqlite3_exec(adjDb, "CREATE TABLE IF NOT EXISTS Titles (id INTEGER PRIMARY KEY, title TEXT COLLATE NOCASE)", NULL, NULL, &sErrMsg);
    sqlite3_exec(adjDb, "CREATE TABLE IF NOT EXISTS Links (id INTEGER PRIMARY KEY, link_from_id INTEGER, link_to_id INTEGER)", NULL, NULL, &sErrMsg);
    //prepare statements for inserting titles and links
    std::string titleStatement = "INSERT INTO Titles VALUES (NULL, @Title)";
    std::string linkStatement = "INSERT INTO Links VALUES(NULL, @fromID, @toID)";
    //std::string titleQueryStr = "SELECT * FROM Titles WHERE title = @title";
    std::string titleQueryStr = "SELECT * FROM Titles WHERE title = @title";
    std::string linkQueryStr = "SELECT * FROM Links WHERE link_from_id = @fromID AND link_to_id = @toID"; 
    sqlite3_prepare_v2(adjDb, titleStatement.c_str(), maxSize, &insertTitle, NULL);
    sqlite3_prepare_v2(adjDb, linkStatement.c_str(), maxSize, &insertLink, NULL);
    sqlite3_prepare_v2(adjDb, titleQueryStr.c_str(), maxSize, &titleQuery, NULL);
    sqlite3_prepare_v2(adjDb, linkQueryStr.c_str(), maxSize, &linkQuery, NULL);

    std::string line;
    //read in list of forbidden phrases
    reader.open(namespaceFile);
    std::getline(reader, line);
    int namespaceCount = std::stoi(line);
    int forbiddenPages = 0, writtenPages = 0, writtenTitles = 0, writtenLinks = 0; 
    int existingLinkCount = 0, existingTitleCount = 0;
    std::string* namespaces = new std::string[namespaceCount]; 
    for(int i = 0; i < namespaceCount; i++)
    {
        std::getline(reader, line);
        //std::cout << "line: " << line << "\n";
        namespaces[i] = line;
    }

    reader.close();
    //output line if string isn't there, otherwise stop analyzing it and move on
    reader.open(parsedInputFile);
    
    //begin sql transaction
    sqlite3_exec(adjDb, "BEGIN TRANSACTION", NULL, NULL, &sErrMsg);
    //prepare both insert queries that will be used
    //statement 
    while(std::getline(reader, line))
    {
        int currentId;
        bool write = checkForbidden(line, namespaces, namespaceCount);
        if(write)
        {
            //check if it's a title or a link
            bool title = isTitle(line);
            if(title)//it's a title, we're on a new adj list
            {
                line = parseTitle(line);
                currentId = getTitleId(adjDb, insertTitle, titleQuery, line, &existingTitleCount);
                if(currentId == 0)
                    std::cout << "id is 0 for title: " << line << "\n";
                writtenTitles++; 
            }
            else if(!title)//its a link
            {
                line = parseLink(line);
                //insert into db if it's not there...
                int toId = getTitleId(adjDb, insertTitle, titleQuery, line, &existingTitleCount);
                if(toId == 0)
                    std::cout << "id is 0 for toId: " << line << "\n";
                int linkId = getLinkId(adjDb, insertLink, linkQuery, currentId, toId, &existingLinkCount);
                if(linkId == 0)
                    std::cout << "id is 0 for link: " << line << "to ID: " << currentId << "\n";
                writtenLinks++;
            }
            //write to db
            
            //std::cout << "writing" << line << "\n";
            writtenPages++;
            //std::cout << "w\n";
            //writer << line << "\n";
        }
        else if(!write)
        {
            //std::cout << "forbidding" << line << "\n";
            if(log)
                forbiddenWriter << line;
            forbiddenPages++;
        }
    }

    //finalize prepared statements
    sqlite3_finalize(insertTitle);
    sqlite3_finalize(insertLink);
    sqlite3_finalize(titleQuery);
    sqlite3_finalize(linkQuery);
    //end sqlite transaction
    sqlite3_exec(adjDb, "END TRANSACTION", NULL, NULL, &sErrMsg);


    std::cout << "\nforbid: " << forbiddenPages << "\n";
    std::cout << "wrote: " << writtenPages << "\n";
    std::cout << "titles: " << writtenTitles << "\n";
    std::cout << "links: " << writtenLinks << "\n";
    std::cout << "pages existing: " << existingTitleCount << "\n";
    std::cout << " links existing: " << existingLinkCount << "\n";
    std::cout << "total: " << forbiddenPages + writtenPages << "\n";
    forbiddenWriter.close();
    reader.close();
    
    //now we actually process the data
    //for each page
        //get list of adj pages
    writer.open(outputFile);
    writer.close();
}

bool checkForbidden(std::string line, std::string *namespaces, int namespaceCount)
{
    bool write = true;
    //need to check for colon and others separately, because article name may have colon
    //std::cout << line << "\n";
    if(line.find(":") != std::string::npos) //if there's a colon, see if any of the non-articles are present
    {
        //std::cout << "found a colon in line: " << line << "\n";
        for(int i = 0; i < namespaceCount; i++)
        {
            //std::cout << "searching for: " << namespaces[i];
            if(line.find(namespaces[i]) != std::string::npos) //if a forbidden word is found
            {
                write = false;
            }
        }
    }
    return write;
}

bool isTitle(std::string line)
{
    // | <> [] are never in article title
    //check if there's a [, it's a link
    //otherwise it's a title
    if(line.find("[") != std::string::npos) //if there's a [
        return false;
    else
        return true;
}

std::string parseTitle(std::string line)
{
    //we want everything between <title> and </title> tags
    //so, everything for position 6, string length will be length - 15 
    //because <title></title> is 15 characters
    return line.substr(7, line.length()-15);     
}

std::string parseLink(std::string line)
{
    //first thing is to evaluate if there's a | 
    int horPos = line.find("|");
    if(horPos != -1) //if there's a |
    {
        //std::cout << "had a | \n";
        //part before | is the title being linked to
        //so, from position 2 (because of [[ with length horPos - 2 
        return line.substr(2, horPos-2);
    }
    //if there's no |
    //std::cout << "no |\n";
    //return substring from position 2 with length len-4
    return line.substr(2, line.length() - 4);
}

int getTitleId(sqlite3 *adjDb, sqlite3_stmt *insertTitle, sqlite3_stmt *titleQuery, std::string line, int *existsCounter) 
{
    //insert into db if it's not there
    sqlite3_bind_text(titleQuery, 1, line.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_step(titleQuery);
    if(sqlite3_data_count(titleQuery) > 0) //if there was a result
    {
        //std::cout << "dataCount > 0 for title: " << line << "\n";
        int id = sqlite3_column_int(titleQuery, 0);
        //std::cout << "column 0 datatype: " << sqlite3_column_decltype(titleQuery, 0) << "is: " << sqlite3_column_int(titleQuery, 0) << "\n";
        sqlite3_clear_bindings(titleQuery);
        sqlite3_reset(titleQuery);
        (*existsCounter)++;
        //std::cout << "id is: " << id << "\n";
        return id;
    }
    else if(sqlite3_data_count(titleQuery) == 0)
    {
        //std::cout << "dataCount == 0  for title: " << line << "\n";
        sqlite3_bind_text(insertTitle, 1, line.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(insertTitle);
        sqlite3_clear_bindings(insertTitle);
        sqlite3_reset(insertTitle);
        //reset the initial id query
        sqlite3_clear_bindings(titleQuery);
        sqlite3_reset(titleQuery);
        int id = sqlite3_last_insert_rowid(adjDb);
        //std::cout << "id is: " << id << "\n";
        return id;
    }
    return 0;

}

int getLinkId(sqlite3 *adjDb, sqlite3_stmt *insertLink, sqlite3_stmt *linkQuery, int fromId, int toId, int *existsCounter)
{
    //insert into db if it's not there
    sqlite3_bind_int(linkQuery, 1, fromId);
    sqlite3_bind_int(linkQuery, 2, toId);
    sqlite3_step(linkQuery);
    //std::cout << "step code was: " << rc << "\n";
    if(sqlite3_data_count(linkQuery) > 0) //if there was a result
    {
        //std::cout << "datacount > 0 for link: " << fromId << " to " << toId << "\n";
        int id = sqlite3_column_int(linkQuery, 0);
        //std::cout << "column 0 datatype: " << sqlite3_column_decltype(linkQuery, 0) << "is: " << sqlite3_column_int(linkQuery, 0) << "\n";
        sqlite3_clear_bindings(linkQuery);
        sqlite3_reset(linkQuery);
        (*existsCounter)++;
        return id;
    }
    else if(sqlite3_data_count(linkQuery) == 0)
    {
        sqlite3_bind_int(insertLink, 1, fromId);
        sqlite3_bind_int(insertLink, 2, toId);
        sqlite3_step(insertLink);
        sqlite3_clear_bindings(insertLink);
        sqlite3_reset(insertLink);
        //reset id query
        sqlite3_clear_bindings(linkQuery);
        sqlite3_reset(linkQuery);
        return sqlite3_last_insert_rowid(adjDb);
    }
    return 0;
}
