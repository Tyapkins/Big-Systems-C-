#ifndef HOMEWORK_5_FACTORY_H
#define HOMEWORK_5_FACTORY_H

#include <memory>
#include <vector>
#include "RandomNumber.h"

class TFactory {
    class TImpl;
    std::unique_ptr<const TImpl> Impl;
public:
    TFactory();
    ~TFactory();
    [[nodiscard]] std::unique_ptr<TRandomNumberGenerator> CreateObject(
            const std::string& name,
            const std::any& pars) const;
    [[nodiscard]] std::vector<std::string> GetAvailableObjects() const;
};

#endif //HOMEWORK_5_FACTORY_H
