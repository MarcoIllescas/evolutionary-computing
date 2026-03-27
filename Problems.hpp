#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include "IOptimizationProblem.hpp"
#include <cmath>

// Sphere Function
class SphereFunction : public IOptimizationProblem {
public:
    ProblemConfiguration getConfiguration() const override {
        return {
            30,
            16,
            -5.2f,
            5.2,
            MINIMIZE
        };
    }

    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        float sum = 0.0f;

        for (unsigned int i = 0; i < numGenes; i++) {
            sum += std::pow(realValues[i], 2);
        }
        return sum;
    }
};

class PracticeFunction : public IOptimizationProblem {
public:
    ProblemConfiguration getConfiguration() const override {
        return {
            2,
            16,
            -6.28f,
            6.28f,
            MAXIMIZE
        };
    }

    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        float x = realValues[0];
        float y = realValues[1];

        float exponent = - (std::pow(x + 1.0f, 2) + std::pow(y - 3.14f, 2)) / 25.0f;

        float term1 = 10.0f * std::exp(exponent);
        float term2 = std::cos(2.0f * x);
        float term3 = std::sin(2.0f * y);

        return term1 + term2 + term3;
    }
};

#endif