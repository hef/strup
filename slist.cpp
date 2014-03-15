#include "slist.h"
#include <assert.h>
#include <future>


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
    nodes.head = nullptr;
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

    std::async([&]{
        std::chrono::seconds duration(3);
        std::this_thread::sleep_for(duration);
        nodes.push(new node<int>(8));
    });

    pNode = nodes.waitPop();
    assert( 8 == pNode->data );

    return 0;
}
