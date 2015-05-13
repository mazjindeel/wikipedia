#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream reader;
    std::ofstream writer;
    std::string line;
    //read in list of forbidden phrases
    reader.open("namespaces.txt");
    std::getline(reader, line);
    int namespaceCount = std::stoi(line); 
    std::string* namespaces = new std::string[namespaceCount]; 
    for(int i = 0; i < namespaceCount; i++)
    {
        std::getline(reader, line);
        namespaces[i] = line;
    }
    reader.close();
    //output line if string isn't there, otherwise stop analyzing it and move on
    reader.open("wikiParsed.txt");
    writer.open("hddWikipedia/wikiFullyParsed.txt");
    while(std::getline(reader, line))
    {
        bool write = true;
        //need to check for colon and others separately, because article name may have colon
        if(line.find(":") == std::string::npos) //if there's a colon, see if any of the non-articles are present
        {
            for(int i = 0; i < namespaces->size(); i++)
            {
                if(line.find(namespaces[i]) != std::string::npos) //if a forbidden word is found
                {
                    write = false;
                }
            }
        }
        if(write)
        {
            //std::cout << "writing" << line << std::endl;
            std::cout << "w\n";
            writer << line << std::endl;
        }
        else if(!write)
        {
            //this never runs?
            std::cout << "forbidding" << line << std::endl;
        }
    }
    reader.close();
    writer.close();
}
