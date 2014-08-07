#include "graph.h"
#include "node.h"
#include <algorithm>

void graph::addWordPair(std::string first, std::string second)
{
    node* firstNode = nullptr;
    node* secondNode = nullptr;

    for( auto n: nodes )
    {
        if(n->word == first)
        {
            firstNode = n;
        }
        if(n->word == second)
        {
            secondNode = n;
        }
    }
    
    /*auto comparitor = [](node* a, node* b){ return a->word < b->word;  };
    
    std::sort(nodes.begin(), nodes.end(), comparitor);
    
    auto finder = [](node* element, std::string value){ return element->word < value;};
    auto x = std::lower_bound(nodes.begin(), nodes.end(), first, finder);
    firstNode = *x;
    secondNode = *std::lower_bound(nodes.begin(), nodes.end(), second, finder);*/


    if(!firstNode)
    {
        firstNode = new node(first);
        nodes.push_back(firstNode);
    }

    if(!secondNode)
    {
        secondNode = new node(second);
        nodes.push_back(secondNode);
    }
    firstNode->addNext(secondNode);

}

void graph::addStartWord(std::string word)
{
    addWordPair("", word);
}

void graph::addEndWord(std::string word)
{
    addWordPair(word, ".");
}


std::string graph::getSentence() const
{
    node* start;
    for(auto n: nodes)
    {
        if(n->word == "")
        {
            start = n;
            break;
        }
    }

    std::string result;
    auto i = start->getNext();
    result += i->word;
    i = i->getNext();
    for( ; i && i->word != "." ; i = i->getNext())
    {
        result += " ";
        result += i->word;
    }
    result += ".";
    return result;
}
