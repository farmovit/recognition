#ifndef ALGORITHMFACTORY_H
#define ALGORITHMFACTORY_H

#include "AlgorithmInterface.h"
#include <memory>
#include <vector>

class AlgorithmFactory 
{
public:
    AlgorithmFactory();
    ~AlgorithmFactory() = default;
    std::unique_ptr<AlgorithmInterface> evaluationAlgorithm(const std::string &algorithmName);
private:
    std::vector<std::unique_ptr<AlgorithmInterface>> mAlgorithms;
};

#endif // ALGORITHMFACTORY_H
