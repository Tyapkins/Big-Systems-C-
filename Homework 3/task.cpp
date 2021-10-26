#include <iostream>
#include <memory>

template <class T>
struct custom_allocator : public std::allocator<T> {
    custom_allocator() noexcept {
        std::cout << "Custom allocator creation and 'new' call" << std::endl;
        T::operator new(sizeof(T));
        std::cout << "'New' was called but without 'new T'" << std::endl;
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
