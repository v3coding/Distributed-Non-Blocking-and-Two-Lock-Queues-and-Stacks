#include "../common/allocator.h"

#define LFENCE asm volatile("lfence" : : : "memory")
#define SFENCE asm volatile("sfence" : : : "memory")

template<class P>
struct pointer_t {
    P* ptr;

    P* address(){
        long long ptr_val = reinterpret_cast<long long>(ptr);
        long long new_ptr_val = ptr_val & ((1ULL << 48) - 1);
        P* new_ptr = reinterpret_cast<int*>(new_ptr_val);
        printf("address = %llx\n", new_ptr_val);
        return new_ptr;
    }
    uint count(){
        uintptr_t ip = reinterpret_cast<uintptr_t>(ptr); // cast pointer to uintptr_t
        int counter = static_cast<int>((ip >> 48) & 0xFFFF); // extract counter from the top 16 bits
        ptr = reinterpret_cast<P*>(ip); // cast back to int*
        std::cout << "Counter : " << ((ip >> 48) & 0xFFFF) << '\n';

        counter = (counter + 65534) & 0xFFFF;
        ip |= static_cast<uintptr_t>(counter) << 48;
        ptr = reinterpret_cast<int*>(ip); // cast back to int*
        std::cout << "Counter after increment: " << ((ip >> 48) & 0xFFFF) << '\n';
        return ((ip >> 48) & 0xFFFF);
    }
};

template <class T>
class Node
{
    T value;
    Node<T>* next;
    
};

template <class T>
class NonBlockingQueue
{
    CustomAllocator my_allocator_;
public:
    
    NonBlockingQueue() : my_allocator_()
    {
        std::cout << "Using NonBlockingQueue\n";
    }

    void initQueue(long t_my_allocator_size){
        std::cout << "Using Allocator\n";
        my_allocator_.initialize(t_my_allocator_size, sizeof(Node<T>));
        // Initialize the queue head or tail here
        Node<T>* newNode = (Node<T>*)my_allocator_.newNode();
        my_allocator_.freeNode(newNode);

        int num = 99;
        pointer_t<int> pointer;
        pointer.ptr = &num;
        pointer.address();
        pointer.count();

        pointer.count();

        std::cout << "num end = " << num << "\n";
    }

    void enqueue(T value)
    {
        // Use LFENCE and SFENCE as mentioned in pseudocode
    }

    bool dequeue(T *value)
    {
        // Use LFENCE and SFENCE as mentioned in pseudocode
        bool ret_value = false;
        return ret_value;
    }

    void cleanup()
    {
        my_allocator_.cleanup();
    }

};

