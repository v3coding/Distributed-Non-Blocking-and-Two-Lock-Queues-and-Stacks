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

        lock1.lock();
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        q_head = q_tail = newNode;
        lock1.unlock();

    }

    void enqueue(T value)
    {
       // std::cout << "enqueue\n";

        lock1.lock();
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->value = value;
        newNode->next = NULL;
        q_tail->next = newNode;
        q_tail = newNode;
        lock1.unlock();
    }

    bool dequeue(T *value)
    {
      //  std::cout << "dequeue\n";

        lock1.lock();
        Node<T>* oldHead = q_head;
        *value = q_head->value;

        if(q_head->next == NULL){
            q_head = NULL;
            q_tail = NULL;
            return 0;
        } else{
            q_head = q_head->next;
        }

        lock1.unlock();

        my_allocator_.freeNode(oldHead);
        return 1;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};