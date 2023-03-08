#include "../common/allocator.h"
#include <mutex>

template <class T>
class Node
{
    public :
    T value;
    Node<T>* next;
};

template <class T>
class OneLockQueue
{
    Node<T>* q_head;
    Node<T>* q_tail;
    CustomAllocator my_allocator_;
    std::mutex lock1;

public:

    OneLockQueue() : my_allocator_()
    {
        std::cout << "Using OneLockQueue\n";
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";

        inserted = 0;
        removed = 0;

        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        q_head = q_tail = newNode;
    }

    void enqueue(T value)
    {
        lock1.lock();
        inserted++;
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->value = value;
        newNode->next = NULL;
        q_tail->next = newNode;
        q_tail = newNode;
        lock1.unlock();
    }

    bool dequeue(T *value)
    {
        lock1.lock();
        removed++;
        Node<T>* node = q_head;
        Node<T>* new_head = q_head->next;

        if(new_head == NULL){
            lock1.unlock();
            return 0;
        }

        *value = new_head->value;

        q_head = new_head;

        my_allocator_.freeNode(&node);

        lock1.unlock();

        return 1;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};