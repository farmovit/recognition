#include "EvaluationDataAdapter.h"
#include "EvaluationDataProvider.h"
#include "EvaluationDataProviderFromBinary.h"

EvaluationDataAdapter::EvaluationDataAdapter(std::experimental::filesystem::path &&evaluatedFileSign,
                                             std::vector<std::experimental::filesystem::path> &&referenceFilesSign) {
    parseData(std::move(evaluatedFileSign), std::move(referenceFilesSign));
}

void EvaluationDataAdapter::parseData(std::experimental::filesystem::path &&evaluatedFileSign, 
                                      std::vector<std::experimental::filesystem::path> &&referenceFilesSign) {
    auto dataProvider = std::make_unique<EvaluationDataProviderFromBinary>();
    dataProvider->parse(std::move(evaluatedFileSign));
    mEvaluatedData = dataProvider->moveData();
    for (auto &&file: referenceFilesSign) {
        dataProvider->parse(std::move(file));
    }
    mReferenceData = dataProvider->moveData();
}

std::vector<std::vector<EvaluationData> > EvaluationDataAdapter::referenceData() const{
    return mReferenceData;
}

std::vector<std::vector<EvaluationData> > EvaluationDataAdapter::testData() const {
    return mEvaluatedData;
}
