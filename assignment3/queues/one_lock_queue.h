#include "../common/allocator.h"

template <class T>
class Node
{
    T value;
    Node<T>* next;
};

template <class T>
class OneLockQueue
{
    Node<T>* q_head;
    Node<T>* q_tail;
    CustomAllocator my_allocator_;

public:

    OneLockQueue() : my_allocator_()
    {
        std::cout << "Using OneLockQueue\n";
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        q_head = q_tail = newNode;

    }

    void enqueue(T value)
    {
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->value = T;
        node->next = NULL;

        q_tail->next = newNode;
        q_tail = newNode;
    }

    bool dequeue(T *value)
    {

        Node<T>* node = q_head;
        Node<T>* new_head = q_head->next;

        if(new_head == NULL){
            return FALSE;
        }

        T *p_value = new_head->next;

        q_head = new_head;

        my_allocator_.freeNode(newNode);

        return TRUE;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};