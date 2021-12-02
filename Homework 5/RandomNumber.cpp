#include "RandomNumber.h"

PoissonNumberGenerator::PoissonNumberGenerator(std::any par) {
    num = std::any_cast<double>(par);
    distribution.param(std::poisson_distribution<int>::param_type(num));
    std::random_device dev;
    generator.seed(dev());
}

double PoissonNumberGenerator::Generate() const {
    return distribution(generator);
}


BernoulliNumberGenerator::BernoulliNumberGenerator(std::any par) {
    num = std::any_cast<double>(par);
    distribution.param(std::bernoulli_distribution::param_type(num));
    std::random_device dev;
    generator.seed(dev());
}

double BernoulliNumberGenerator::Generate() const {
    return distribution(generator);
}


GeometricNumberGenerator::GeometricNumberGenerator(std::any par) {
    num = std::any_cast<double>(par);
    distribution.param(std::geometric_distribution<int>::param_type(num));
    std::random_device dev;
    generator.seed(dev());
}

double GeometricNumberGenerator::Generate() const {
    return distribution(generator);
}



FiniteNumberGenerator::FiniteNumberGenerator(std::any par) {
    auto nums = std::any_cast<fin_distr>(par);
    values = nums.first;
    poss = nums.second;
    distribution.param(std::discrete_distribution<int>::param_type(poss.begin(), poss.end()));
    std::random_device dev;
    generator.seed(dev());
}

double FiniteNumberGenerator::Generate() const {
    return values[distribution(generator)];
}

