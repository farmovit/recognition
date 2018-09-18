#include <iostream>

#include "CommandLineDataProvider.h"
#include "EvaluationDataAdapter.h"
#include "AlgorithmInterface.h"

int main(int argc, char *argv[]) try {
    auto data = std::make_unique<CommandLineDataProvider>(argc, argv);
    auto algorithm = data->algorithm();
    auto adapter = EvaluationDataAdapter(data->evaluatedSign(), data->referenceSigns());
    algorithm->setTestData(adapter.testData());
    algorithm->setReferenceData(adapter.referenceData());
    algorithm->evaluate();
    std::cout << algorithm->result() << std::endl;
    return 0;
} catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
