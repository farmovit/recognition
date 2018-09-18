#ifndef ALGORITHMINTERFACE_H
#define ALGORITHMINTERFACE_H

#include "EvaluationData.h"
#include <string>
#include <vector>

class AlgorithmInterface 
{
public:
    virtual ~AlgorithmInterface() = default;
    virtual void evaluate() = 0;
    virtual std::string name() const = 0;
    virtual double result() const = 0;
    virtual void setReferenceData(std::vector<std::vector<EvaluationData> > &&referenceData) = 0;
    virtual void setTestData(std::vector<std::vector<EvaluationData> > &&testData) = 0;
};

#endif // ALGORITHMINTERFACE_H
