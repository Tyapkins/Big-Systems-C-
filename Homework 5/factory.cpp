#include "factory.h"
#include <map>
#include <algorithm>

class TFactory::TImpl {


    class ICreator {
    public:
        virtual ~ICreator() = default;
        [[nodiscard]] virtual std::unique_ptr<TRandomNumberGenerator> Create(std::any pars) const = 0;
    };

    using TCreatorPtr = std::shared_ptr<ICreator>;
    using TRegisteredCreators = std::map<std::string, TCreatorPtr>;
    TRegisteredCreators RegisteredCreators;

public:

    template<class TCurrentObject>
    class TCreator : public ICreator {
        [[nodiscard]] std::unique_ptr<TRandomNumberGenerator> Create(std::any pars) const override {
            return std::make_unique<TCurrentObject>(pars);
        }
    };


    TImpl() { RegisterAll(); }

    template<typename T>
    void RegisterCreator(const std::string &name) {
        RegisteredCreators[name] = std::make_shared<TCreator<T>>();
    }

    void RegisterAll() {
        RegisterCreator<PoissonNumberGenerator>("poisson");
        RegisterCreator<BernoulliNumberGenerator>("bernoulli");
        RegisterCreator<GeometricNumberGenerator>("geometric");
        RegisterCreator<FiniteNumberGenerator>("finite");
    }

    static std::function<bool(std::any)> check_fun(const std::string& name)
    {
        if (name == "poisson")
            return [](std::any par)
            {
                auto actual_par = std::any_cast<double>(par);
                return (actual_par > 0);
            };
        else if ((name == "bernoulli") || (name == "geometric"))
            return [](std::any par)
            {
                auto actual_par = std::any_cast<double>(par);
                return (actual_par >= 0) && (actual_par <= 1);
            };
        else if (name == "finite")
            return [](std::any par){
                auto fin_nums = std::any_cast<fin_distr>(par);
                auto poss = fin_nums.second;
                auto all_pos = std::all_of(poss.begin(), poss.end(), [](int i){return (i >= 0) && (i <= 1);});
                auto sum_poss = std::accumulate(poss.begin(), poss.end(), 0.0);
                return (fin_nums.first.size() == poss.size()) && (all_pos) && (sum_poss == 1.0);
            };
        else
            return [](const std::any& par) {return true;};
    }

    [[nodiscard]] std::unique_ptr<TRandomNumberGenerator> CreateObject(const std::string &n, const std::any& pars) const {
        auto creator = RegisteredCreators.find(n);
        if (creator == RegisteredCreators.end()) {
            return nullptr;
        }
        auto is_corr = check_fun(creator->first);
        if (is_corr(pars))
            return creator->second->Create(pars);
        else
            return nullptr;
    }

    [[nodiscard]] std::vector<std::string> GetAvailableObjects() const {
        std::vector<std::string> result;
        for (const auto &creatorPair : RegisteredCreators) {
            result.push_back(creatorPair.first);
        }
        return result;
    }
};

    std::unique_ptr <TRandomNumberGenerator> TFactory::CreateObject(const std::string &n, const std::any& pars) const {
        return Impl->CreateObject(n, pars);
    }

    TFactory::TFactory() : Impl(std::make_unique<TFactory::TImpl>()) {}

    TFactory::~TFactory() = default;

    std::vector <std::string> TFactory::GetAvailableObjects() const {
        return Impl->GetAvailableObjects();
    }

template <typename T>
std::pair<double, double> check_distrib(const std::string& name, T par, int iter_num = 10000)
{
        TFactory factory;
        const std::any& true_par(par);
        auto obj = factory.CreateObject(name, true_par);
        double sum = 0;
        for (int i = 0; i < iter_num; i++)
            sum += obj->Generate();
        return std::make_pair(sum/iter_num, obj->GetMean());
}

int main()
{
    std::vector<double> pois_vals = {0.1, 0.5, 1.0, 2.5, 5.0, 10.0, 17.1, 25.3, 93.546};
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


