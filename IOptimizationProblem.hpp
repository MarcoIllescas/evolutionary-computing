#ifndef IOPTIMIZATIONPROBLEM_HPP
#define IOPTIMIZATIONPROBLEM_HPP

enum OptimizationType {
    MAXIMIZE,
    MINIMIZE
};

// ===== STRUCT WITH PROBLEM RULES ===== //
struct ProblemConfiguration {
    unsigned int dimensions;
    unsigned int bitsPerDimension;
    float lowerLimit;
    float upperLimit;
    OptimizationType type;
};

class IOptimizationProblem {
public:
    virtual ~IOptimizationProblem() = default;

    virtual ProblemConfiguration getConfiguration() const = 0;

    virtual float evaluate(const unsigned int* intValues, const float* realValues, unsigned int numGenes) const = 0;
};

#endif