#include "edge.h"

edge::edge(std::shared_ptr<node> next)
: next(next)
, count(1)
{

}

std::shared_ptr<node> edge::getNext()
{
    return next;
}

