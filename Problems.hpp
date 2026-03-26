#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include "IOptimizationProblem.hpp"
#include <cmath>

// Problem 1: Maximize the parabola
class MaximizeParabola : public IOptimizationProblem {
public:
    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        const float x = static_cast<float>(intValues[0]);

        return 250.0f - std::pow(x - 155.0f, 2);
    }
};

// Problem 2: Finding root
class RootFinding : public IOptimizationProblem {
public:
    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        const float x = static_cast<float>(intValues[0]);

        return (x * x) - (23.0f * x) - 50.0f;
    }
};

// Problem 3: Multivariable
class MultiVariableProblem : public IOptimizationProblem {
public:
    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        if (numGenes < 2) return 0.0f;

        float x0 = realValues[0];
        float x1 = realValues[1];

        return 250.0f - std::pow(x0 - 64.0f, 2) - std::pow(x1 - 128.0f, 2);
    }
};

#endif