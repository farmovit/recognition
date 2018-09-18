#include "NaiveAlgorithm.h"

#include <iostream>
#include <cmath>

std::string NaiveAlgorithm::name() const {
    return "naive";
}

double NaiveAlgorithm::result() const {
    return mResult;
}

void NaiveAlgorithm::setReferenceData(std::vector<std::vector<EvaluationData> > &&referenceData) {
    mReferenceData = std::move(referenceData);
}

void NaiveAlgorithm::setTestData(std::vector<std::vector<EvaluationData> > &&testData) {
    mTestData = std::move(testData);
}

void NaiveAlgorithm::evaluate() {
    auto referenceGapSize = 0;
    for (const auto &data: mReferenceData) {
        for (const auto &point: data) {
            if (point.y == -1) {
                ++referenceGapSize;
            }
        }
    }
    referenceGapSize /= mReferenceData.size();
    auto evalGapSize = 0;
    for (const auto &data: mTestData) {
        for (const auto &point: data) {
            if (point.y == -1) {
                ++evalGapSize;
            }
        }
    }
    evalGapSize /= mTestData.size();
    mResult = 1 - std::fabs(static_cast<int>(evalGapSize) - static_cast<int>(referenceGapSize)) / std::max(evalGapSize, referenceGapSize);
}
