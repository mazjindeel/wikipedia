#include <iostream>
#include <fstream>
#include <string>

int main()
{
    //paths for all output files
    std::string parsedInputFile = "wikiParsed.txt";
    std::string parsedOutputFile = "wikiFullyParsed.txt"; 
    parsedOutputFile = "hddWikipedia/wikiFullyParsed.txt"; 
    std::string forbiddenFile = "forbiddenPages.txt"; 
    forbiddenFile = "hddWikipedia/forbiddenPages.txt";
    std::string namespaceFile = "namespaces.txt";

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
