#include <string>
#include <iostream>
#include <vector>
#include <sstream>
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

    template<typename T>
    void iteration(T beg, T end);

};

template<typename T>
void Printer::iteration(T beg, T end)
{
    for (auto i = beg; i != end; i++)
    {
        this->format(*i);
        if (std::next(i) != end)
            final_line << ", ";
    }
}

template <typename T1, typename T2>
Printer& Printer::format(std::pair<T1, T2> param)
{
    final_line << "(";
    this->format(param.first);
    final_line << ", ";
    this->format(param.second);
    final_line << ")";
    return *this;
}

template <typename T>
Printer& Printer::format(T&& param)
{//since nothing was said about special string handling, their output does not change
    final_line << param;
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
            (this->format(args),
                    final_line << (++counter < max_size ? ", " : "")),
            ...
        );
   },
        param);
    final_line << " )";
    return *this;
}

template <typename T>
Printer& Printer::format(std::vector<T> param)
{
    final_line << "[ ";
    iteration(param.begin(), param.end());
    final_line << " ]";
    return *this;
}

template <typename T>
Printer& Printer::format(std::set<T> param)
{
    final_line << "{ ";
    iteration(param.begin(), param.end());
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

    std::vector<std::set<std::pair<int,int>>> vec = { {{4,5}, {2, 6}, {10, 7}}, {{123, 11},
                                                                                 {14, 14}},
                                                      {{123, 123}, {1, 1}, {2, 2}, {10,10}}};

    std::tuple<decltype(vec), float, int, std::set<std::vector<std::string>>> super_vec = {vec, 0.5, 11,
                                                                                           {{"pum", "pum", "pam"},
                                                                                            {"check", "test", "oh no"},
                                                                                            {"let's see..."}
                                                                                           }};

    std::string s1 = Printer().format(" vector: ").format(v).str();
    std::cout << s1 << std::endl;
// " vector: [ (1, 4), (5, 6) ]"
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
// "( xyz, 1, 2 ) ! 0"
    std::cout << s2 << std::endl;

    std::string s3 = Printer().format(" Scary one: ").format(vec).str();
    std::cout << s3 << std::endl;

    std::string s4 = Printer().format("Behold! Something terrible: ").format(super_vec)
            .format("\n And that's not all! ").format(14).format(" ").format(s1).format(" and... ").format(0.5).str();

    std::cout << s4 << std::endl;
}
