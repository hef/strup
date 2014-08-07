#ifndef node_h
#define node_h

#include <string>
#include <vector>
#include <memory>

class edge;
class node {
    friend class graph;
    std::string word;
    std::vector< edge* > edges;
    int getTotal() const;
    edge* pick(int index);
public:
    node( std::string word );
    void addNext( node* next);
    node* getNext();
};
#endif // #ifndef node_h
