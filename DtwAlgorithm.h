#ifndef DTWEVALUATIONALGORITHM_H
#define DTWEVALUATIONALGORITHM_H

#include "AlgorithmInterface.h"

template<class T>
struct TimeSeries {
    std::vector<T> xSeries{};
    std::vector<T> ySeries{};
    std::vector<T> xVelocities{};
    std::vector<T> yVelocities{};
};

class DtwEvaluationAlgorithm : public AlgorithmInterface
{
public:
    DtwEvaluationAlgorithm() = default;
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

#endif // DTWEVALUATIONALGORITHM_H
