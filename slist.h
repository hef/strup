#ifndef SLIST_H
#define SLIST_H
#include <atomic>
#include <mutex>
#include <condition_variable>

template <typename T>
class slist {
public:
    void push(T*);
    T* pop();
    T* waitPop();
//private:
    std::atomic<T*> head;
    // waitablestuff
    std::mutex m;
    std::condition_variable cv;
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
    cv.notify_one();
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

template<typename T>
T* slist<T>::waitPop()
{
    T* result;
    std::unique_lock<std::mutex> lk(m);

    cv.wait(lk, [&]{ 
        result = pop();
        return result != nullptr;
    });
    return result;
}

#endif // #ifndef SLIST_H
