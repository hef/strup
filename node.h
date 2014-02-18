#ifndef node_h
#define node_h

#include <string>
#include <vector>
#include <memory>

class edge;
class node {
    friend class graph;
    std::string word;
    std::vector< std::shared_ptr<edge> > edges;
    int getTotal() const;
    std::shared_ptr<edge> pick(int index);
public:
    node(std::string word);
    void addNext( std::shared_ptr<node> next);
    std::shared_ptr<node> getNext();
};
#endif // #ifndef node_h
