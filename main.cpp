#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <memory>
#include "graph.h"


int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    graph g;

    std::ifstream file;
    //file.open("input.txt", std::ios::in);
    file.open("pg11.txt", std::ios::in);
    assert(file.is_open());
    
    char ch;
    std::string lastWord;
    std::string currentWord;
    while(!file.eof())
    {
        file.get(ch);
        switch(ch)
        {
        case '\'':
            if(currentWord != "")
            {
                currentWord += ch;
                break;
            }
        case '\"':
        case '\t':
        case '\r':
        case '\n':
        case ' ':
        case ',':
        case '-':
        case ':':
        case ';':
        case '(':
        case ')':
            if(currentWord != "")
            {
                g.addWordPair(lastWord, currentWord);
                lastWord = currentWord;
                currentWord = "";
            }
            break;
        case '.':
        case '!':
        case '?':
            g.addWordPair(lastWord, currentWord);
            g.addEndWord(currentWord);
            lastWord = "";
            currentWord = "";
            break;
        default:
            currentWord += ch;
            break;
        }    
    }
    file.close();

    std::cout << g.getSentence() << std::endl;
    return 0;
}
