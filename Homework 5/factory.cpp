#include "factory.h"

// factory implementation
class TFactory::TImpl {


    class ICreator {
    public:
        virtual ~ICreator() = default;
        virtual std::unique_ptr<TRandomNumberGenerator> Create(std::any pars) const = 0;
    };

    using TCreatorPtr = std::shared_ptr<ICreator>;
    using TRegisteredCreators = std::map<std::string, TCreatorPtr>;
    TRegisteredCreators RegisteredCreators;

public:

    template<class TCurrentObject>
    class TCreator : public ICreator {
            std::unique_ptr<TRandomNumberGenerator> Create(std::any pars) const override {
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


    // function generator for checking if parameters are correct

    static std::function<bool(std::any)> check_fun(const std::string& name)
    {
        if (name == "poisson")
            return [](std::any par)
            {
                if (par.type() != typeid(double))
                    return false;
                auto actual_par = std::any_cast<double>(par);
                return (actual_par > 0);
            };
        else if ((name == "bernoulli") || (name == "geometric"))
            return [](std::any par)
            {
                if (par.type() != typeid(double))
                    return false;
                auto actual_par = std::any_cast<double>(par);
                return (actual_par >= 0) && (actual_par <= 1);
            };
        else if (name == "finite")
            return [](std::any par){
                if (par.type() != typeid(fin_distr))
                    return false;
                auto fin_nums = std::any_cast<fin_distr>(par);
                auto poss = fin_nums.second;
                auto all_pos = std::all_of(poss.begin(), poss.end(), [](int i){return (i >= 0) && (i <= 1);});
                auto sum_poss = std::accumulate(poss.begin(), poss.end(), 0.0);
                return (fin_nums.first.size() == poss.size()) && (all_pos) && (sum_poss == 1.0);
            };
        else
            return [](const std::any& par) {return true;};
    }

    // Checking if parameters are correct, and create object, if they are.

    std::unique_ptr<TRandomNumberGenerator> CreateObject(const std::string &n, const std::any& pars) const {
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

    std::vector<std::string> GetAvailableObjects() const {
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


// template function for checking the distribution
template <typename T>
std::pair<double, double> check_distrib(const std::string& name, T par, int iter_num)
{
        TFactory factory;
        const std::any& true_par(par);
        auto obj = factory.CreateObject(name, true_par);
        double sum = 0;
        if (obj == nullptr)
            return std::make_pair(0.0, 0.0);
        for (int i = 0; i < iter_num; i++)
            sum += obj->Generate();
        return std::make_pair(sum/iter_num, obj->GetMean());
}


// tell compiler, which template arguments will be used. Otherwise it will not compile (because of the template)
auto test1 = check_distrib<double>;
auto test2 = check_distrib<fin_distr>;