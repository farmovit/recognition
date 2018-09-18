#ifndef COMMANDLINEDATAPROVIDER_H
#define COMMANDLINEDATAPROVIDER_H

#include "DataProvider.h"

class AlgorithmInterface;
class CommandLineDataProvider : public DataProvider
{
public:
    explicit CommandLineDataProvider(const int &argc, char **argv);
    std::unique_ptr<AlgorithmInterface> algorithm() override;
    std::experimental::filesystem::path evaluatedSign() const override;
    std::vector<std::experimental::filesystem::path> referenceSigns() const override;
private:
    void takeAlgorithm();
    void takeSigns();
    void checkCommandLine();
    std::vector<std::string> getParametersForArgument(const std::pair<std::string, int> &key);
private:
    std::vector<std::string> mArguments;
    std::experimental::filesystem::path mEvaluatedSign;
    std::vector<std::experimental::filesystem::path> mReferenceSigns;
    std::unique_ptr<AlgorithmInterface> mAlgorithm;
};

#endif // COMMANDLINEDATAPROVIDER_H
