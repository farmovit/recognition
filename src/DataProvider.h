#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <memory>
#if __has_include(<filesystem>)
    #include <filesystem>
#else
    #include <experimental/filesystem>
#endif

class AlgorithmInterface;
class DataProvider
{
public:
    virtual ~DataProvider() = default;
    virtual std::unique_ptr<AlgorithmInterface> algorithm() = 0;
    virtual std::experimental::filesystem::path evaluatedSign() const = 0;
    virtual std::vector<std::experimental::filesystem::path> referenceSigns() const = 0;
};

#endif // DATAPROVIDER_H
