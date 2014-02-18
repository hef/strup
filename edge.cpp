#include "edge.h"

edge::edge(std::weak_ptr<node> next)
: next(next)
, count(1)
{

}

std::shared_ptr<node> edge::getNext()
{
    return next.lock();
}

