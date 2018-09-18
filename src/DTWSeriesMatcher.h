#ifndef DTWSERIESMATCHER_H
#define DTWSERIESMATCHER_H

#include <vector>
#include <array>

template<class T>
class DTWSeriesMatcher {
public:
    DTWSeriesMatcher() = default;
    std::pair<std::vector<T>, std::vector<T>> matchSeries(std::pair<std::vector<T>, std::vector<T>> &&series);
    T dtwPath() const;
private:
    std::pair<std::vector<T>, std::vector<T>> calculateBacktrack(std::pair<std::vector<T>, std::vector<T>> &&series);
    std::pair<std::vector<T>, std::vector<T>> stretchSeries(std::pair<std::vector<T>, std::vector<T>> &&series, std::pair<std::vector<T>, std::vector<T>> &&indexes);
    int chooseOptimalDirection(std::array<double, 3> &directions);
private:
    enum Step { 
        eDiagonal, 
        eLeft, 
        eUp 
    };
    T mDtwPath;
};
#endif // DTWSERIESMATCHER_H
