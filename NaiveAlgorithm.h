#ifndef NAIVEALGORITHM_H
#define NAIVEALGORITHM_H

#include "AlgorithmInterface.h"

class NaiveAlgorithm : public AlgorithmInterface 
{
public:
    NaiveAlgorithm() = default;
    void evaluate() override;
    std::string name() const override;
    double result() const override;
    void setReferenceData(std::vector<std::vector<EvaluationData> > &&referenceData) override;
    void setTestData(std::vector<std::vector<EvaluationData> > &&testData) override;
private:
    double mResult;
    std::vector<std::vector<EvaluationData> > mReferenceData;
    std::vector<std::vector<EvaluationData> > mTestData;
};

#endif // NAIVEALGORITHM_H
