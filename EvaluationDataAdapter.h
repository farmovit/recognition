#ifndef EVALUATIONDATAADAPTER_H
#define EVALUATIONDATAADAPTER_H

#include "EvaluationData.h"
#if __has_include(<filesystem>)
    #include <filesystem>
#else
    #include <experimental/filesystem>
#endif

class EvaluationDataAdapter
{
public:
    explicit EvaluationDataAdapter(std::experimental::filesystem::path &&evaluatedFileSign, 
                                   std::vector<std::experimental::filesystem::path> &&referenceFilesSign);
    std::vector<std::vector<EvaluationData> > testData() const;
    std::vector<std::vector<EvaluationData> > referenceData() const;
private:
    void parseData(std::experimental::filesystem::path &&evaluatedFileSign, 
                   std::vector<std::experimental::filesystem::path> &&referenceFilesSign);
private:
    std::vector<std::vector<EvaluationData> > mEvaluatedData;
    std::vector<std::vector<EvaluationData> > mReferenceData;
};

#endif // EVALUATIONDATAADAPTER_H
