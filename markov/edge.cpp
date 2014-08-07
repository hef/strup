#include "edge.h"

edge::edge(node* next)
: next(next)
, count(1)
{

}

node* edge::getNext()
{
    return next;
}

