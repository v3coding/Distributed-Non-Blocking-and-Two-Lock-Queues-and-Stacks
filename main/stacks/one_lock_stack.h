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
class OneLockStack
{

    Node<T>* top;
    std::mutex lock1;

    CustomAllocator my_allocator_;
public:
    OneLockStack() : my_allocator_()
    {
        std::cout << "Using OneLockStack\n";
    }

    void initStack(long t_my_allocator_size)
    {
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->next = NULL;
        top = newNode;
        // Perform any necessary initializations
    }

    /**
     * Create a new node with value `value` and update it to be the top of the stack.
     * This operation must always succeed.
     */
    void push(T value)
    {
        lock1.lock();
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        newNode->value = value;
        newNode->next = top;
        top = newNode;
        lock1.unlock();
    }

    /**
     * If the stack is empty: return false.
     * Otherwise: copy the value at the top of the stack into `value`, update
     * the top to point to the next element in the stack, and return true.
     */
    bool pop(T *value)
    {
        lock1.lock();
        removed++;

        Node<T>* node = top;
        Node<T>* new_top = top->next;

        if(new_top == NULL){
            lock1.unlock();
            return 0;
        }

        *value = node->value;
        top = new_top;
        my_allocator_.freeNode(top);
        lock1.unlock();
        return 1;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }
};
