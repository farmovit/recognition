#include "DtwAlgorithm.h"
#include "DTWSeriesMatcher.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <limits>
#include <cmath>

std::string DtwEvaluationAlgorithm::name() const {
    return "dtw";
}

double DtwEvaluationAlgorithm::result() const {
    return mResult;
}

void DtwEvaluationAlgorithm::setReferenceData(std::vector<std::vector<EvaluationData> > &&referenceData) {
    mReferenceData = std::move(referenceData);
}

void DtwEvaluationAlgorithm::setTestData(std::vector<std::vector<EvaluationData> > &&testData) {
    mTestData = std::move(testData);
}

template<class T>
void normalizeSeries(std::vector<T> &series) {
    auto extremums = std::minmax_element(series.begin(), series.end());
    auto amplitude = *(extremums.second) - *(extremums.first);
    if (std::fabs(amplitude) <= std::numeric_limits<T>::epsilon()) {
        amplitude = 1;
    }
    double average = std::accumulate(series.begin(), series.end(), 0.0) / static_cast<double>(series.size());
    for (auto &point: series) {
        point = (point - average) / amplitude;
    }
}

template<class T>
std::vector<double> calculateVelocity(const std::vector<T> &points) {
    std::vector<double> velocity;
    velocity.reserve(points.size() - 1);
    for(size_t i = 1; i < points.size(); ++i) {
        velocity.emplace_back(points[i] - points[i - 1]);
    }
    return velocity;
}

template<class T>
auto preprocess(const std::vector<std::vector<EvaluationData>> &dataList) {
    std::vector<TimeSeries<T>> timeSeries;
    for (const auto &data: dataList) {
        auto dataSize = data.size();
        std::vector<T> points_x;
        points_x.reserve(dataSize);
        std::vector<T> points_y;
        points_y.reserve(dataSize);
        for (const auto &point: data) {
            if (point.y == -1) {
                continue;
            }
            points_x.push_back(point.x);
            points_y.push_back(point.y);
        }
        normalizeSeries(points_x);
        normalizeSeries(points_y);
        timeSeries.push_back({points_x, points_y, calculateVelocity(points_x), calculateVelocity(points_y)});
    }
    return timeSeries;
}

double calculateSeriesSimilarity(std::vector<std::pair<std::vector<double>, std::vector<double>>> &&series) {
    double numerator = 0;
    double denominatorFirstHalf = 0;
    double denominatorSecondHalf = 0;
    for (const auto &elementDimension: series) {
        auto referenceSeries = elementDimension.first;
        auto testSeries = elementDimension.second;
        auto numberOfPoints = referenceSeries.size();
        if (testSeries.size() != numberOfPoints) {
            throw std::runtime_error("Series length is not equal!");
        }
        double referenceAverage = std::accumulate(referenceSeries.begin(), referenceSeries.end(), 0.0) / static_cast<double>(numberOfPoints);
        double testAverage = std::accumulate(testSeries.begin(), testSeries.end(), 0.0) / static_cast<double>(numberOfPoints);
        double numeratorSum = 0;
        double denominatorFirstSum = 0;
        double denominatorSecondSum = 0;
        for (size_t i = 0; i < numberOfPoints; ++i) {
            double testHalf = testSeries[i] - testAverage;
            double referenceHalf = referenceSeries[i] - referenceAverage;
            numeratorSum += testHalf * referenceHalf;
            denominatorFirstSum += std::pow(testHalf, 2);
            denominatorSecondSum += std::pow(referenceHalf, 2);
        }
        numerator += numeratorSum;
        denominatorFirstHalf += denominatorFirstSum;
        denominatorSecondHalf += denominatorSecondSum;
    }
    numerator *= numerator;
    auto denominator = denominatorFirstHalf * denominatorSecondHalf;
    if (std::fabs(denominator) <= std::numeric_limits<double>::epsilon()) {
        denominator = 1;
    }
    return numerator/denominator;
}

void DtwEvaluationAlgorithm::evaluate() {
    auto referenceSeries = preprocess<double>(mReferenceData);
    auto testSeries = preprocess<double>(mTestData);
    std::vector<double> similarities;
    for (auto &&reference: referenceSeries) {
        for (auto &&test: testSeries) {
            auto firstSeriesForMatch = std::make_pair(reference.xSeries, test.xSeries);
            auto secondSeriesForMatch = std::make_pair(reference.ySeries, test.ySeries);
            auto firstSeriesVelocityForMatch = std::make_pair(reference.xVelocities, test.xVelocities);
            auto secondSeriesVelocityForMatch = std::make_pair(reference.yVelocities, test.yVelocities);
            DTWSeriesMatcher<double> matcher;
            firstSeriesForMatch = matcher.matchSeries(std::move(firstSeriesForMatch));
            secondSeriesForMatch = matcher.matchSeries(std::move(secondSeriesForMatch));
            firstSeriesVelocityForMatch = matcher.matchSeries(std::move(firstSeriesVelocityForMatch));
            secondSeriesVelocityForMatch = matcher.matchSeries(std::move(secondSeriesVelocityForMatch));
            similarities.emplace_back(calculateSeriesSimilarity({firstSeriesForMatch, secondSeriesForMatch, firstSeriesVelocityForMatch, secondSeriesVelocityForMatch}));
        }
    }
    mResult = std::accumulate(similarities.begin(), similarities.end(), 0.0) / similarities.size();
}

