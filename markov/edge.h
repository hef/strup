#ifndef edge_h
#define edge_h
#include <memory>
class node;
class edge {
    friend class node;
    node* next;
    int count;
public:
    edge(node* node);
    node* getNext();
};
#endif // #ifndef edge_h
