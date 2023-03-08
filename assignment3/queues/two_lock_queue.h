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
class TwoLockQueue
{
    Node<T>* q_head;
    Node<T>* q_tail;
    CustomAllocator my_allocator_;
    std::mutex headLock;
    std::mutex tailLock;

public:
    TwoLockQueue() : my_allocator_()
    {
        std::cout << "Using TwoLockQueue\n";
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        // Initialize the queue head or tail here

        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        q_head = q_tail = newNode;
        my_allocator_.freeNode(newNode);
    }

    void enqueue(T value)
    {
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->value = value;
        newNode->next = NULL;

        tailLock.lock();
        q_tail->next = newNode;
        q_tail = newNode;
        tailLock.unlock();
   
    }

    bool dequeue(T *value)
    {
        headLock.lock();
        Node<T>* node = q_head;
        Node<T>* new_head = q_head->next;
        headLock.unlock();

        if(new_head == NULL){
            return 0;
        }

        headLock.lock();
        *value = new_head->value;
        q_head = new_head;
        headLock.unlock();

        my_allocator_.freeNode(&node);

        return 1;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};