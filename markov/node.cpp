#include "node.h"
#include "edge.h"
#include <random>
#include <assert.h>

node::node(std::string word)
: word(word)
{

}

int node::getTotal() const
{
    int total = 0;
    for(auto e: edges)
    {
        total += e->count;
    }
    return total;
}

edge* node::pick(int index)
{
    assert( index >= 0 && "Pick index too small.");
    assert( index < getTotal() && "Pick index too big.");
    for( auto e: edges )
    {
        index -= e->count;
        if( index <= 0)
        {
            return e;
        }
    }
    assert(false && "pick index was too big");
    return nullptr;
}

void node::addNext( node* next )
{
    for(auto e: edges)
    {
        if( next->word == e->next->word )
        {
            ++e->count;
            return;
        }
    }
    edges.push_back( new edge(next) );
}

std::random_device generator;
node* node::getNext()
{
    if(edges.size())
    {
        std::uniform_int_distribution<int> distribution(0, getTotal()-1);
        int index = distribution(generator);
        return pick(index)->getNext();
    }
    else
    {
        assert(false && "Should not get here.");
        return nullptr;
    }
}
