#ifndef SLIST_H
#define SLIST_H
#include <atomic>

template <typename T>
class slist {
public:
    void push(T*);
    T* pop();
private:
    std::atomic<T*> head;
};

template <typename T>
void slist<T>::push(T* n)
{
    T* old_head = nullptr;
    do {
        old_head = this->head;
        n->slist_next = head;
    }
    while( !std::atomic_compare_exchange_weak(&head, &old_head, n) );
}

template<typename T>
T* slist<T>::pop()
{
    T* new_head = nullptr;
    T* current_head = nullptr;
    do {
        current_head = this->head;
        if(current_head)
            new_head = current_head->slist_next;
    }
    while( !std::atomic_compare_exchange_weak(&head, &current_head, new_head) );
    return current_head;
}

#endif // #ifndef SLIST_H
