#include "CommandLineDataProvider.h"
#include "AlgorithmFactory.h"
#include <iostream>
#include <algorithm>

namespace {
// the pair of key and number of minimal required arguments
const std::pair<std::string, int> testKey("-t", 2);
const std::pair<std::string, int> algorithmKey("-a", 1);
const std::string defaultAlgorithm("dtw");
const int minimumArgsRequirement = 3;
} // namespace

void usage() {
    std::cout << "usage: <sign [-a algorithm_name <dtw/naive>] -t path_to_sign_1 ... path_to_sign_N path_to_tested_sign\n";
}

CommandLineDataProvider::CommandLineDataProvider(const int &argc, char **argv) 
    : mArguments(argv, argv + argc) {
    checkCommandLine();
    takeSigns();
    takeAlgorithm();
}

void CommandLineDataProvider::checkCommandLine() {
    if (mArguments.size() < minimumArgsRequirement + 1) {
        usage();
        throw std::runtime_error("At least " + std::to_string(minimumArgsRequirement) + " arguments required");
    }
}

std::vector<std::string> CommandLineDataProvider::getParametersForArgument(const std::pair<std::string, int> &key) {
    auto keyArgument = std::find(mArguments.begin(), mArguments.end(), key.first);
    if (keyArgument == mArguments.end()) {
        return {};
    }
    ++keyArgument;
    auto nextKeyOrEnd = std::find_if(keyArgument, mArguments.end(), [](const std::string &argument) {
        return argument.compare(0, 1, "-") == 0;
    });
    auto pathsNumber = std::distance(keyArgument, nextKeyOrEnd);
    if (pathsNumber < key.second) {
        usage();
        throw std::runtime_error("key " + testKey.first + " must have at least " + std::to_string(testKey.second) + " parameters");
    }
    std::vector<std::string> result;
    result.reserve(static_cast<size_t>(pathsNumber));
    for (auto argumentIter = keyArgument; argumentIter != nextKeyOrEnd; ++argumentIter) {
        result.emplace_back(*argumentIter);
    }
    return result;
}

void CommandLineDataProvider::takeSigns() {
    auto testArguments = getParametersForArgument(testKey);
    if (testArguments.empty()) {
        usage();
        throw std::runtime_error("Program must have key " + testKey.first);
    }
    mEvaluatedSign = std::move(testArguments.back());
    std::move(testArguments.begin(), testArguments.end( ) - 1, std::back_inserter(mReferenceSigns));
}

void CommandLineDataProvider::takeAlgorithm() {
    std::string algorithmName(defaultAlgorithm);
    auto algorithmArgument = getParametersForArgument(algorithmKey);
    if (!algorithmArgument.empty()) {
        algorithmName = algorithmArgument.at(0);
    }
    auto evaluationAlgorithmFactory = AlgorithmFactory();
    mAlgorithm = evaluationAlgorithmFactory.evaluationAlgorithm(algorithmName);
}

std::vector<std::experimental::filesystem::path> CommandLineDataProvider::referenceSigns() const {
    return mReferenceSigns;
}

std::unique_ptr<AlgorithmInterface> CommandLineDataProvider::algorithm() {
    return std::move(mAlgorithm);
}

std::experimental::filesystem::path CommandLineDataProvider::evaluatedSign() const {
    return mEvaluatedSign;
}
