#include <iostream>
#include <memory>

template <class T1, class T2 = T1>
struct custom_allocator
{
    typedef T1 value_type;
    typedef T1* pointer;
    typedef T1& reference;


    custom_allocator() = default;
    template <class U>
    custom_allocator (const custom_allocator<U>&) noexcept {};

    T1 * allocate (std::size_t n)
    {
        return reinterpret_cast<T1*>(T2::operator new(n));
    }

    void deallocate (T1* p, std::size_t n)
    {
        return T2::operator delete(p, n);
    }
};




class A {
public:

    static void * operator new(size_t size) {
        std::cout << "operator new!" << std::endl;
        return ::operator new(size);
    }
    static void operator delete(void *p, size_t size) {
        std::cout << "operator delete!" << std::endl;
        return ::operator delete(p);
    }
};

int main() {
    auto sp = std::make_shared<A>();
    auto check = std::allocate_shared<A>(custom_allocator<A>());
    return 0;
}
