#include "factory.h"

template<typename T>
void print_distr(const std::string& name, T values)
{
    std::cout << "Checking " << name << " distribution" << std::endl;

    for (auto val: values) {
        auto pair_val = check_distrib(name, val);
        std::cout << pair_val.first << " : " << pair_val.second << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int main()
{
    std::vector<double> pois_vals = {0.1, 0.5, 1.0, 2.5, 5.0, 10.0, 17.1, 25.3, 93.546, 7};
    print_distr("poisson", pois_vals);

    std::vector<double> bern_vals = {};
    int max_num = 10;
    for (int i = 0; i <= max_num; i++)
        bern_vals.push_back(double(i)/max_num);

    print_distr("bernoulli", bern_vals);
    print_distr("geometric", bern_vals);

    std::vector<fin_distr> fin_check =
            {{{1, 5, 10}, {0.1, 0.5, 0.4}}, {{0, 1}, {0.3, 0.7}}, {{100, 55, 32}, {0.3, 0.3, 0.4}},
             {{2, 5, 6, 10, 100}, {0.25, 0.4, 0.15, 0.15, 0.05}}, {{2, 5, 6, 10, 100}, {0.2, 0.2, 0.2, 0.2, 0.2}},
             {{10, 100, 1000}, {0.3, 0.3, 0.4}}, {{10, 100, 1000}, {0.6, 0.35, 0.05}}};

    print_distr("finite", fin_check);
    
    return 0;
}

