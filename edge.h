#ifndef edge_h
#define edge_h
#include <memory>
class node;
class edge {
    friend class node;
    std::shared_ptr<node> next;
    int count;
public:
    edge(std::shared_ptr<node> node);
    std::shared_ptr<node> getNext();
};
#endif // #ifndef edge_h
