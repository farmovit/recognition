#ifndef EVALUATIONDATAPROVIDER_H
#define EVALUATIONDATAPROVIDER_H

#include "EvaluationData.h"
#include <vector>
#if __has_include(<filesystem>)
    #include <filesystem>
#else
    #include <experimental/filesystem>
#endif

class EvaluationDataProvider 
{
public:
    virtual ~EvaluationDataProvider() = default;
    virtual std::vector<std::vector<EvaluationData>> moveData() = 0;
    virtual void parse(std::experimental::filesystem::path &&file) = 0;
};

#endif // EVALUATIONDATAPROVIDER_H
