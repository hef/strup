#include "slist.h"
#include <assert.h>


template <typename T>
struct node
{
    T data;
    node(T data) : data(data){}
    node<T>* slist_next; 
};

int main(int, char* [])
{
    slist<node<int>> nodes;
    node<int>* pNode = nullptr;
    pNode = nodes.pop();
    assert( nullptr == pNode);
    nodes.push(new node<int>(1));
    nodes.push(new node<int>(2));
    nodes.push(new node<int>(3));
    pNode = nodes.pop();
    assert( 3 == pNode->data);
    delete pNode;
    pNode = nodes.pop();
    assert( 2 == pNode->data);
    delete pNode;
    pNode = nodes.pop();
    assert( 1 == pNode->data );
    delete pNode;
    pNode = nodes.pop();
    assert(nullptr == pNode );

    return 0;
}
