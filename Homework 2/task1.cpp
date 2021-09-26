#include <iostream>
#include <vector>
#include <sstream>

int main() {



    // different compile results of STL container!

    std::vector<bool> v{};

    /* due to optimization, bool container uses one bit for each element
     * to address bits, special object is used
     * returned object is temporary
     * so, it can not bind to lvalue references */

    /* если же изъясняться на родном русском языке, то
     * контейнер из булевских значений оптимизирован, поэтому
     * каждый элемент занимает ровно 1 бит (вместо 1 байта, как это
     * делает обычный bool). Для адресации одного бита используется специальный
     * прокси-объект, который является временным и по этой причине не может
     * связываться с lvalue-значениями */

    // this one compiles perfectly as auto&& binds to rvalue
    // компилируется, поскольку auto&& -- rvalue.
    for (auto&& x : v)
    {
        std::cout << x << std::endl;
    }
    // this one does not compile as auto& binds to lvalue
    // не компилируется, так как присутствует неконстантная ссылка на временный объект
    for (auto& x: v)
    {
        std::cout << x << std::endl;
    }
}

