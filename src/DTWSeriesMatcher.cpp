#include "DTWSeriesMatcher.h"
#include <cmath>
#include <algorithm>

// Allowed declarations
template class DTWSeriesMatcher<int>;
template class DTWSeriesMatcher<unsigned>;
template class DTWSeriesMatcher<double>;
template class DTWSeriesMatcher<short>;

template<class T>
std::pair<std::vector<T>, std::vector<T>> DTWSeriesMatcher<T>::matchSeries(std::pair<std::vector<T>, std::vector<T> > &&series) {
    auto backtrack = calculateBacktrack(std::move(series));
    return stretchSeries(std::move(series), std::move(backtrack));
}

template<class T>
auto calculateEuclideanDistance(T &&firstPoint, T &&secondPoint) {
    return std::sqrt(std::pow((firstPoint - secondPoint), 2));
}

template<class T>
int DTWSeriesMatcher<T>::chooseOptimalDirection(std::array<double, 3> &directions) {
    auto direction = (directions[eLeft] < directions[eDiagonal]) ? eLeft : eDiagonal;
    direction = (directions[eUp] < directions[direction]) ? eUp : direction;
    return direction;
}

template<class T>
void transformIndexesToSeries(std::vector<T> &&series, std::vector<T> &&indexes) {
    for (auto &index: indexes) {
        index = series.at(index);
    }
}

template<class T>
std::pair<std::vector<T>, std::vector<T>> DTWSeriesMatcher<T>::stretchSeries(std::pair<std::vector<T>, std::vector<T>> &&series, std::pair<std::vector<T>, std::vector<T>> &&indexes) {
    transformIndexesToSeries(std::move(series.first), std::move(indexes.first));
    transformIndexesToSeries(std::move(series.second), std::move(indexes.second));
    return std::make_pair(std::move(indexes.first), std::move(indexes.second));
}

template<class T>
std::pair<std::vector<T>, std::vector<T>> DTWSeriesMatcher<T>::calculateBacktrack(std::pair<std::vector<T>, std::vector<T>> &&series) {
    auto firstTimeSeriesSize = series.first.size();
    auto secondTimeSeriesSize = series.second.size();
    if (firstTimeSeriesSize < 1 || secondTimeSeriesSize < 1) {
        throw std::runtime_error("Time series must have at least 1 point");
    }
    std::vector<T> firstTimeSeriesIdx;
    std::vector<T> secondTimeSeriesIdx;
    std::vector<std::vector<double>> cost(firstTimeSeriesSize + 1, std::vector<double>(secondTimeSeriesSize + 1));
    for (size_t i = 0; i <= firstTimeSeriesSize; ++i) {
        cost[i][0] = std::numeric_limits<double>::max();
    }
    for (size_t j = 1; j <= secondTimeSeriesSize; ++j) {
        cost[0][j] = std::numeric_limits<double>::max();
    }
    cost[1][1] = calculateEuclideanDistance(series.first[0], series.second[0]);
    for (size_t i = 1; i <= firstTimeSeriesSize; ++i) {
        for (size_t j = 1; j <= secondTimeSeriesSize; ++j) {
            if (i == 1 && j == 1) {
                continue;
            }
            auto localCost = calculateEuclideanDistance(series.first[i-1], series.second[j-1]);
            std::array<double, 3> directions{cost[i - 1][j - 1], cost[i][j - 1], cost[i - 1][j]};
            for (auto &direction: directions) {
                direction += localCost;
            }
            auto direction = chooseOptimalDirection(directions);
            cost[i][j] = directions[static_cast<size_t>(direction)];
            cost[i - 1][j - 1] = direction;
        }
    }
    auto i = firstTimeSeriesSize - 1;
    auto j = secondTimeSeriesSize - 1;
    firstTimeSeriesIdx.push_back(i);
    secondTimeSeriesIdx.push_back(j);
    while(!(i == 0 && j == 0)) {
        int localCost = cost[i][j];
        switch(localCost) {
        case eDiagonal:
            --i; --j; 
            break;
        case eLeft:
            --j;
            break;
        case eUp:
            --i;
            break;
        default:
            throw std::runtime_error("Bad direction matrix!");
        }
        firstTimeSeriesIdx.push_back(i);
        secondTimeSeriesIdx.push_back(j);
    }
    std::reverse(firstTimeSeriesIdx.begin(), firstTimeSeriesIdx.end());
    std::reverse(secondTimeSeriesIdx.begin(), secondTimeSeriesIdx.end());
    mDtwPath = cost[firstTimeSeriesSize][secondTimeSeriesSize];
    return std::make_pair(firstTimeSeriesIdx, secondTimeSeriesIdx);
}

template<class T>
T DTWSeriesMatcher<T>::dtwPath() const {
    return mDtwPath;
}
