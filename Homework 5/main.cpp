#include "factory.h"

int main()
{
    std::vector<double> pois_vals = {0.1, 0.5, 1.0, 2.5, 5.0, 10.0, 17.1, 25.3, 93.546, 7};
    std::cout << "Checking Poisson distribution:" << std::endl;

    for (auto val: pois_vals) {
        auto pam = check_distrib("poisson", val);
        std::cout << pam.first << " : " << pam.second << std::endl;
    }

    std::cout << std::endl << std::endl;

    std::vector<double> bern_vals = {};
    int max_num = 10;
    for (int i = 0; i <= max_num; i++)
        bern_vals.push_back(double(i)/max_num);


    std::cout << "Checking Bernoulli distribution:" << std::endl;
    for (auto val: bern_vals) {
        auto pam = check_distrib("bernoulli", val);
        std::cout << pam.first << " : " << pam.second << std::endl;
    }

    std::cout << std::endl << std::endl;

    std::cout << "Checking Geometric distribution:" << std::endl;
    for (auto val: bern_vals) {
        auto pam = check_distrib("geometric", val);
        std::cout << pam.first << " : " << pam.second << std::endl;
    }

    std::cout << std::endl << std::endl;


    std::vector<fin_distr> fin_check =
            {{{1, 5, 10}, {0.1, 0.5, 0.4}}, {{0, 1}, {0.3, 0.7}}, {{100, 55, 32}, {0.3, 0.3, 0.4}},
             {{2, 5, 6, 10, 100}, {0.25, 0.4, 0.15, 0.15, 0.05}}, {{2, 5, 6, 10, 100}, {0.2, 0.2, 0.2, 0.2, 0.2}},
             {{10, 100, 1000}, {0.3, 0.3, 0.4}}, {{10, 100, 1000}, {0.6, 0.35, 0.05}}};

    std::cout << "Checking Finite distribution:" << std::endl;
    for (const auto& val : fin_check) {
        auto pam = check_distrib("finite", val);
        std::cout << pam.first << " : " << pam.second << std::endl;
    }

    return 0;
}

