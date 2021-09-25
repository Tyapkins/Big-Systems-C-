#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <tuple>
#include <type_traits>
#include <set>

struct Printer {
    std::stringstream final_line;

    std::string str() const
    {
        return final_line.str();
    }

    template<typename T>
    Printer& format(T&&);

    template <typename T1, typename T2>
    Printer& format(std::pair<T1, T2>);

    template <typename T>
    Printer& format(std::vector<T>);

    template <typename T>
    Printer& format(std::set<T>);

    template <typename... Args>
    Printer& format(std::tuple<Args...>);

    //template <typename T1, typename T2>
    //void iter_print(T1<T2>);
};



/*template <typename T1, typename T2>
void Printer::iter_print(T1<T2> param)
{
    for (auto i = param.begin(); i<param.end(); i++)
    {
        this -> format(*i);
        if (std::next(i) != param.end())
            final_line << ", ";
    }
}*/

template <typename T1, typename T2>
Printer& Printer::format(std::pair<T1, T2> param)
{
    final_line << "(";
    this->format(param.first);
    final_line << ", ";
    this->format(param.second);
    final_line << ")";
}

template <typename T>
Printer& Printer::format(T&& pam)
{
    final_line << pam;
    return *this;
}

template <typename... Args>
Printer& Printer::format(std::tuple<Args...> param)
{
    int counter = 0, max_size = std::tuple_size<decltype(param)>::value;
    final_line << "( ";
    std::apply([this, &counter, max_size](auto&&... args)
    {
        (
            (this->format(args), final_line << (++counter < max_size ? ", " : "")), ...);}, param);
    final_line << " )";
    return *this;
}

template <typename T>
Printer& Printer::format(std::vector<T> param)
{
    final_line << "[ ";
    for (auto i = param.begin(); i<param.end(); i++)
    {
        this -> format(*i);
        if (std::next(i) != param.end())
            final_line << ", ";
    }
    final_line << " ]";
    return *this;
}

template <typename T>
Printer& Printer::format(std::set<T> param)
{
    final_line << "{ ";
    for (auto i = param.begin(); i<param.end(); i++)
    {
        this -> format(*i);
        if (std::next(i) != param.end())
            final_line << ", ";
    }
    final_line << " }";
    return *this;
}

template<typename T>
std::string format(const T& t) {
    return Printer().format(t).str();
}

int main() {

    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};


    std::string s1 = Printer().format(" vector: ").format(v).str();
    std::cout << s1 << std::endl;
// " vector: [ (1, 4), (5, 6) ]"
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
// "( xyz, 1, 2 ) ! 0"
    std::cout << s2 << std::endl;
}


