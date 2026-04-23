#ifndef PROBLEMS_HPP
#define PROBLEMS_HPP

#include "IOptimizationProblem.hpp"
#include <cmath>

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

class RosenbrockFunction : public IOptimizationProblem {
public:
    ProblemConfiguration getConfiguration() const override {
        return {
            30,
            16,
            -30.0f,
            30.0f,
            MINIMIZE
        };
    }

    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        float sum = 0.0f;

        for (unsigned int i = 0; i < numGenes-1; i++) {
            float xi = realValues[i];
            float xip = realValues[i+1];

            float term1 = 100.0f * std::pow(xip - xi*xi, 2);
            float term2 = std::pow(xi - 1.0f, 2);

            sum += term1 + term2;
        }

        return sum;
    }
};

class RastriginFunction : public IOptimizationProblem {
public:
    ProblemConfiguration getConfiguration() const override {
        return {
            30,
            16,
            -5.12f,
            5.12f,
            MINIMIZE
        };
    }

    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        const float PI = 3.14159265358979323846f;

        float sum = 0.0f;

        for (unsigned int i = 0; i < numGenes; i++) {
            float x = realValues[i];

            sum += (x * x) - 10.0f * std::cos(2.0f * PI * x) + 10.0f;
        }

        return sum;
    }
};

class GriewankFunction : public IOptimizationProblem {
public:
    ProblemConfiguration getConfiguration() const override {
        return {
            30,
            16,
            -600.0f,
            600.0f,
            MINIMIZE
        };
    }

    float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const override {
        float sumPart = 0.0f;
        float prodPart = 1.0f;

        for (unsigned int i = 0; i < numGenes; i++) {
            float x = realValues[i];
            sumPart += (x * x) / 4000.0f;
            prodPart *= std::cos(x / std::sqrt(float(i + 1)));
        }

        return sumPart - prodPart + 1.0f;
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