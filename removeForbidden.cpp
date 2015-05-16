/* Author: Mazin Jindeel
** Date: 5/15
** Functionality: This utility is designed to remove namespaces from parsed Wikipedia data. 
** Usage: To run this utility, you must provide a minimum of two arguments: the original, parsed file (with namespaces) as well as the output path. However, there are also some switches you can use to configure the program. 
Namespaces: If you wish to override the default namespaces.txt file, it is recommended that you merely add your namespaces to that file. Make sure to update the namespace counter at the top. The provided file removes most main namespaces, as of May 2015. However, if you want to overwrite the file, provide the switch 
-n <fileNameSpaces>.  
Forbidden Pages: This utility also logs any records that are not being included in the final output in a separate file. On slower filesystems or systems, it may be in your best interest to disable this logging feature. To do this, use the following:
-l <true/false>. 
A value of "true" will enable logging, false will disable it
You can also specify a path to the log file with the following switch:
-log <fileName>
*/ 
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
//TODO: args from readme
//arg1 is the 
int main(int argc, char* argv[])
{
    //default values
    //paths for all output files
    std::string parsedInputFile = "wikiParsed.txt";
    std::string parsedOutputFile = "wikiFullyParsed.txt"; 
    parsedOutputFile = "hddWikipedia/wikiFullyParsed.txt"; 
    std::string forbiddenFile = "forbiddenPages.txt"; 
    forbiddenFile = "hddWikipedia/forbiddenPages.txt";
    std::string namespaceFile = "namespaces.txt";
    bool log = true;

    //deal with command line args
    if(argc > 2) //if there was an input/output file specified at least
    {
        parsedInputFile = argv[1];
        parsedOutputFile = argv[2];
        for(int i = 2; i < argc; i++) //iterate through remaining arguments
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
            
        }
    }
    std::ifstream reader;
    std::ofstream writer;
    std::ofstream forbiddenWriter;
    forbiddenWriter.open(forbiddenFile);
    std::string line;
    //read in list of forbidden phrases
    reader.open(namespaceFile);
    std::getline(reader, line);
    int namespaceCount = std::stoi(line);
    int forbiddenPages = 0, writtenPages = 0; 
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
    //writer.open("hddWikipedia/wikiFullyParsed.txt");
    writer.open(parsedOutputFile);
    while(std::getline(reader, line))
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
        if(write)
        {
            //std::cout << "writing" << line << "\n";
            writtenPages++;
            //std::cout << "w\n";
            writer << line << "\n";
        }
        else if(!write)
        {
            //std::cout << "forbidding" << line << "\n";
            if(log)
                forbiddenWriter << line;
            forbiddenPages++;
        }
    }

    std::cout << "forbid: " << forbiddenPages << "\n";
    std::cout << "wrote: " << writtenPages << "\n";
    std::cout << "total: " << forbiddenPages + writtenPages << "\n";
    forbiddenWriter.close();
    reader.close();
    writer.close();
}
