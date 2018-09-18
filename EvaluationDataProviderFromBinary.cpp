#include "EvaluationDataProviderFromBinary.h"
#include <fstream>
#include <iostream>
#include <iterator>

namespace {
const uint32_t wordSize = 2;
} // namespace

void EvaluationDataProviderFromBinary::parse(std::experimental::filesystem::path &&tabFile) {
    if (tabFile.empty()) {
        throw std::runtime_error("EvaluationDataProvider got empty file name!");
    }
    if (!std::experimental::filesystem::exists(tabFile)) {
        throw std::runtime_error("File " + tabFile.string() + " does not exist!");
    }
    std::ifstream file(tabFile.string(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file " + tabFile.string());
    }
    short numberOfSigns;
    file.read(reinterpret_cast<char *>(&numberOfSigns), wordSize);
    if (numberOfSigns <= 0) {
        throw std::runtime_error("File " + tabFile.string() + " consist " + std::to_string(numberOfSigns) + " signs");
    }
    mEvaluationData.reserve(static_cast<size_t>(numberOfSigns));
    for (auto signNumber = 0; signNumber < numberOfSigns; ++signNumber) {
        short numberOfPoints;
        file.read(reinterpret_cast<char *>(&numberOfPoints), wordSize);
        if (numberOfPoints <= 0) {
            throw std::runtime_error("File " + tabFile.string() + " consist " + std::to_string(numberOfPoints) + " points");
        }
        std::vector<EvaluationData> points;
        points.reserve(static_cast<size_t>(numberOfPoints));
        
        std::vector<short> xs(static_cast<size_t>(numberOfPoints));
        file.read(reinterpret_cast<char *>(&xs[0]), static_cast<int>(wordSize) * numberOfPoints);
        
        std::vector<short> ys(static_cast<size_t>(numberOfPoints));
        file.read(reinterpret_cast<char *>(&ys[0]), static_cast<int>(wordSize) * numberOfPoints);
        
        for (auto pointNumber = 0; pointNumber < numberOfPoints; ++pointNumber) {
            points.push_back({xs[static_cast<size_t>(pointNumber)], ys[static_cast<size_t>(pointNumber)]});
        }
        
        mEvaluationData.push_back(points);
    }
}

std::vector<std::vector<EvaluationData>> EvaluationDataProviderFromBinary::moveData() {
    return std::move(mEvaluationData);
}
