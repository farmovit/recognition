#include "AlgorithmFactory.h"
#include "NaiveAlgorithm.h"
#include "DtwAlgorithm.h"

AlgorithmFactory::AlgorithmFactory() {
    mAlgorithms.emplace_back(std::make_unique<NaiveAlgorithm>());
    mAlgorithms.emplace_back(std::make_unique<DtwEvaluationAlgorithm>());
}

std::unique_ptr<AlgorithmInterface> AlgorithmFactory::evaluationAlgorithm(const std::string &algorithmName) {
    for (auto &&algorithm: mAlgorithms) {
        if (algorithm->name() == algorithmName) {
            return std::move(algorithm);
        }
    }
    return std::make_unique<DtwEvaluationAlgorithm>();
}
