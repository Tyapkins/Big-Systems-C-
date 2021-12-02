#include <string>
#include <random>
#include <any>
#include <iostream>

typedef std::vector<double> nums;
typedef std::pair<nums, nums> fin_distr;


class TRandomNumberGenerator {

protected:
    mutable std::mt19937 generator;

public:
    virtual ~TRandomNumberGenerator() = default;
    virtual double Generate() const = 0;
    virtual double GetMean() const = 0;
};

class PoissonNumberGenerator : public TRandomNumberGenerator {

    mutable std::poisson_distribution<int> distribution;
    double num = 0.0;

    public:
        explicit PoissonNumberGenerator(std::any par);
        double Generate() const override;
        double GetMean() const override {return num;}
};


class BernoulliNumberGenerator : public TRandomNumberGenerator {

    mutable std::bernoulli_distribution distribution;
    double num = 0.0;

    public:
        explicit BernoulliNumberGenerator(std::any num);
        double Generate() const override;
        double GetMean() const override {return num;}
};


class GeometricNumberGenerator : public TRandomNumberGenerator {

    mutable std::geometric_distribution<int> distribution;
    double num = 0.0;

    public:
        explicit GeometricNumberGenerator(std::any num);
        double Generate() const override;
        double GetMean() const override {return 1/num - 1;}
};


class FiniteNumberGenerator : public TRandomNumberGenerator {

    nums values, poss;
    mutable std::discrete_distribution<int> distribution;

public:
    explicit FiniteNumberGenerator(std::any nums);
    double Generate() const override;
    double GetMean() const override {return std::inner_product(values.begin(), values.end(), poss.begin(), 0.0);}
};