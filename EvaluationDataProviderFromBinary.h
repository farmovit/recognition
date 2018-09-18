#ifndef EVALUATIONDATAPROVIDERFROMBINARY_H
#define EVALUATIONDATAPROVIDERFROMBINARY_H

#include "EvaluationDataProvider.h"

class EvaluationDataProviderFromBinary : public EvaluationDataProvider
{
public:
    EvaluationDataProviderFromBinary() = default;
    void parse(std::experimental::filesystem::path &&tabFile) override;
    std::vector<std::vector<EvaluationData>> moveData() override;
private:
    std::vector<std::vector<EvaluationData>> mEvaluationData;
};

#endif // EVALUATIONDATAPROVIDERFROMBINARY_H
