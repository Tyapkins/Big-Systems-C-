#ifndef HOMEWORK_5_FACTORY_H
#define HOMEWORK_5_FACTORY_H

#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include "RandomNumber.h"

class TFactory {
    class TImpl;
    std::unique_ptr<const TImpl> Impl;
public:
    TFactory();
    ~TFactory();
    std::unique_ptr<TRandomNumberGenerator> CreateObject(
            const std::string& name,
            const std::any& pars) const;

    std::vector<std::string> GetAvailableObjects() const;
};

template <typename T>
std::pair<double, double> check_distrib(const std::string& name, T par, int iter_num = 10000);

#endif //HOMEWORK_5_FACTORY_H
