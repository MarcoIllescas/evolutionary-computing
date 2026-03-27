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

#endif