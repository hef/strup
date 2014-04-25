#include "graph.h"
#include "node.h"

void graph::addWordPair(std::string first, std::string second)
{
    std::shared_ptr<node> firstNode;
    std::shared_ptr<node> secondNode;

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

    if(!firstNode)
    {
        firstNode = std::make_shared<node>(first);
        nodes.push_back(firstNode);
    }

    if(!secondNode)
    {
        secondNode = std::make_shared<node>(second);
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
    std::shared_ptr<node> start;
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
